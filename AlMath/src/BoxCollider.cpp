#include <Jasper\BoxCollider.h>
#include <Jasper\GameObject.h>
#include <Jasper\Mesh.h>

namespace Jasper {

BoxCollider::BoxCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world)
	:PhysicsCollider(name, mesh, world)
{

}


BoxCollider::~BoxCollider()
{
}

void BoxCollider::Awake()
{
	auto go = GetGameObject();
	auto& trans = go->GetLocalTransform();
	auto& btTrans = trans.GetBtTransform();

	Vector3 extents = m_mesh->GetHalfExtents();

	m_collisionShape = new btBoxShape(btVector3(extents.x, extents.y, extents.z));

	btVector3 inertia;
	m_collisionShape->calculateLocalInertia(Mass, inertia);

	m_defaultMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbci(Mass, m_defaultMotionState, m_collisionShape, inertia);
	m_rigidBody = new btRigidBody(rbci);

	m_world->AddRigidBody(m_rigidBody);
}

}