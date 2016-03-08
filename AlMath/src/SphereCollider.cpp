#include <Jasper\SphereCollider.h>
#include <Jasper\Mesh.h>
#include <Jasper\GameObject.h>
#include <Jasper\Sphere.h>

namespace Jasper {

SphereCollider::SphereCollider(const std::string & name, Mesh * mesh, PhysicsWorld * world)
	:PhysicsCollider(name, mesh, world)
{

}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Awake()
{
	auto go = GetGameObject();
	auto& trans = go->GetLocalTransform();
	auto& btTrans = trans.GetBtTransform();
	float radius = 0.f;		
	auto extents = m_mesh->GetHalfExtents().AsFloatPtr();
	for (int i = 0; i < 3; ++i) {
		float r = fabs(extents[i]);
		if (r > radius) {
			radius = r;
		}
	}				
	m_collisionShape = new btSphereShape(radius);

	btVector3 inertia;
	m_collisionShape->calculateLocalInertia(Mass, inertia);

	m_defaultMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbci(Mass, m_defaultMotionState, m_collisionShape, inertia);
	m_rigidBody = new btRigidBody(rbci);
	m_rigidBody->setRestitution(1.f);

	m_world->AddRigidBody(m_rigidBody);
}

} // namespace Jasper