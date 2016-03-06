#include <Jasper\Camera.h>
#include <Jasper\PhysicsWorld.h>

namespace Jasper {

Vector3 Camera::WORLD_X_AXIS = Vector3(1.0f, 0.0f, 0.0f);
Vector3 Camera::WORLD_Y_AXIS = Vector3(0.0f, 1.0f, 0.0f);
Vector3 Camera::WORLD_Z_AXIS = Vector3(0.0f, 0.0f, 1.0f);

Camera::Camera(Camera::CameraType type)
{
	m_type = type;
	transform.Position = { 0.0f, 1.82f, 0.0f };
}

void Camera::Awake() {
	float mass = 80.f;

	const float width = 0.8f / 2.f;
	const float depth = 0.4f / 2.f;
	const float height = 1.9f / 2.f;

	m_collisionShape = std::make_unique<btCapsuleShape>(width / 2, height * 2.f);
	btDefaultMotionState* ms = new btDefaultMotionState(transform.GetBtTransform());
	btVector3 inertia;
	m_collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbci(mass, ms, m_collisionShape.get(), inertia);
	m_rigidBody = std::make_unique<btRigidBody>(rbci);
	m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_physicsWorld->AddRigidBody(m_rigidBody.get());
}


Camera::~Camera()
{
	if (m_rigidBody->getMotionState() != nullptr) {
		delete m_rigidBody->getMotionState();
	}
}


Matrix4 Camera::GetViewMatrix()
{
	Matrix4 vm = transform.TransformMatrix();
	m_localXAxis = Vector3(vm.mat[0].x, vm.mat[1].x, vm.mat[2].x);
	m_localYAxis = Vector3(vm.mat[0].y, vm.mat[1].y, vm.mat[2].y);
	m_localZAxis = Vector3(vm.mat[0].z, vm.mat[1].z, vm.mat[2].z);
	m_viewVector = -m_localZAxis;
	const auto pos = transform.Position;
	m_rigidBody->getMotionState()->setWorldTransform(transform.GetBtTransform());
	printf("Camera Position: %.3f, %.3f, %.3f Direction: %.3f, %.3f, %.3f \r", pos.x, pos.y, pos.z, m_viewVector.x, m_viewVector.y, m_viewVector.z);
	return vm;
}

Matrix4 Camera::GetCubemapViewMatrix()
{
	// removes the translation components for skybox rendering
	Matrix4 vm = Matrix4::FromTransform(transform);
	vm.mat[0].w = 0.0f;
	vm.mat[1].w = 0.0f;
	vm.mat[2].w = 0.0f;
	return vm;
}

void Camera::Rotate(float pitch, float roll, float yaw)
{
	m_accumPitch += pitch;
	Quaternion orientation = transform.Orientation;

	if (m_accumPitch > 90.f) {
		pitch = 0.f;
		m_accumPitch = 90.f;
	}
	if (m_accumPitch < -90.f) {
		pitch = 0.f;
		m_accumPitch = -90.f;
	}

	Quaternion xrot, yrot;
	if (pitch != 0.f) {
		xrot = Quaternion::FromAxisAndAngle(WORLD_X_AXIS, DEG_TO_RAD(pitch));
		transform.Orientation = transform.Orientation * xrot;
	}

	if (yaw != 0.f) {
		yrot = Quaternion::FromAxisAndAngle(WORLD_Y_AXIS, DEG_TO_RAD(yaw));
		transform.Orientation = yrot * transform.Orientation;	
	}
	//transform.Orientation = Normalize(transform.Orientation);
	//transform.Orientation =  orientation;
}

void Camera::Translate(float x, float y, float z)
{
	Translate({ x, y, z });
}

void Camera::Translate(const Vector3& vec)
{
	Vector3 forwards;
	float z = vec.z * -1.0f;

	if (m_type == CameraType::FIRST_PERSON) {
		forwards = WORLD_Y_AXIS.Cross(m_localXAxis);
		forwards.Normalize();
	}
	else {
		forwards = m_viewVector;
	}
	auto& current = transform.Position;

	/*btVector3 after = current.AsBtVector3();
	after += m_localXAxis.AsBtVector3() * vec.x;
	after += WORLD_Y_AXIS.AsBtVector3() * vec.y;
	after += forwards.AsBtVector3() * z;*/

	/*btTransform from, to;
	from.setIdentity();
	to.setIdentity();
	from.setRotation(transform.Orientation.AsBtQuaternion());
	to.setRotation(transform.Orientation.AsBtQuaternion());
	from.setOrigin(current.AsBtVector3());
	to.setOrigin(after);*/

	if (m_physicsWorld) {
		btVector3 after = current.AsBtVector3();
		after += m_localXAxis.AsBtVector3() * vec.x;
		after += WORLD_Y_AXIS.AsBtVector3() * vec.y;
		after += forwards.AsBtVector3() * z;

		btTransform from, to;
		from.setIdentity();
		to.setIdentity();
		from.setRotation(transform.Orientation.AsBtQuaternion());
		to.setRotation(transform.Orientation.AsBtQuaternion());
		from.setOrigin(current.AsBtVector3());
		to.setOrigin(after);

		btCollisionWorld::ClosestConvexResultCallback cb(current.AsBtVector3(), after);
		cb.m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;

		btConvexShape* cs = static_cast<btConvexShape*>(m_rigidBody->getCollisionShape());
		m_physicsWorld->ConvexSweepTest(cs, from, to, cb);
		if (cb.hasHit()) {
			float epsilon = 0.000001;
			float min = btMax(epsilon, cb.m_closestHitFraction);
			btVector3 newPos = current.AsBtVector3();
			newPos.setInterpolate3(current.AsBtVector3(), after, min);
			transform.Position = Vector3(newPos);
		}
		else {
			transform.Position = Vector3(after);
		}
	}
	else
	{
		current += m_localXAxis * vec.x;
		current += WORLD_Y_AXIS * vec.y;
		current += forwards * z;
		transform.Position = current;
	}

	m_rigidBody->getMotionState()->setWorldTransform(transform.GetBtTransform());

}
}