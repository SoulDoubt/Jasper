#include <Jasper\CapsuleCollider.h>
#include <Jasper\GameObject.h>
#include <Jasper\Mesh.h>

Jasper::CapsuleCollider::CapsuleCollider(const std::string & name, Mesh * mesh, PhysicsWorld * world)
	: PhysicsCollider(name, mesh, world)
{
}

Jasper::CapsuleCollider::~CapsuleCollider()
{
}

void Jasper::CapsuleCollider::Awake()
{
	auto go = GetGameObject();
	auto& trans = go->GetLocalTransform();
	auto& btTrans = trans.GetBtTransform();

	Vector3 extents = m_mesh->GetHalfExtents();
	float radius = std::max(extents.x, extents.z);
	float height = extents.y * 2.0f;

	m_collisionShape = new btCapsuleShape(radius, height);

	btVector3 inertia;
	m_collisionShape->calculateLocalInertia(Mass, inertia);

	m_defaultMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbci(Mass, m_defaultMotionState, m_collisionShape, inertia);
	m_rigidBody = new btRigidBody(rbci);

	m_world->AddRigidBody(m_rigidBody);
}
