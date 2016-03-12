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

	Vector3 halfExtents = m_mesh->GetHalfExtents();
	Vector3 minExtents = m_mesh->GetMinExtents();
	Vector3 maxExtents = m_mesh->GetMaxExtents();

	float halfX = (maxExtents.x - minExtents.x) / 2.0f;
	float halfY = (maxExtents.y - minExtents.y) / 2.0f;
	float halfZ = (maxExtents.z - minExtents.z) / 2.0f;
	
	halfX *= trans.Scale.x;
	halfY *= trans.Scale.y;
	halfZ *= trans.Scale.z;

	float radius = std::max(halfX, halfZ);
	float height = (maxExtents.y - minExtents.y) * trans.Scale.y;
	
	m_collisionShape = new btCapsuleShape(radius, height);

	btVector3 inertia;
	m_collisionShape->calculateLocalInertia(Mass, inertia);

	m_defaultMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbci(Mass, m_defaultMotionState, m_collisionShape, inertia);
	m_rigidBody = new btRigidBody(rbci);

	m_world->AddRigidBody(m_rigidBody);
}
