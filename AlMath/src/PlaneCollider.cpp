#include <Jasper\PlaneCollider.h>
#include <Jasper\GameObject.h>

namespace Jasper {

PlaneCollider::PlaneCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world)
	:PhysicsCollider(name, mesh, world)
{


}


PlaneCollider::~PlaneCollider()
{
}

void PlaneCollider::Awake()
{
	auto go = GetGameObject();
	auto& trans = go->GetLocalTransform();
	auto& btTrans = trans.GetBtTransform();
	m_collisionShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
	m_defaultMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbci(Mass, m_defaultMotionState, m_collisionShape, btVector3(0.f, -1.f, 0.f));	
	m_rigidBody = new btRigidBody(rbci);
	m_rigidBody->setRestitution(Restitution);
	m_rigidBody->setFriction(Friction);
	m_world->AddCollider(this);

}
}