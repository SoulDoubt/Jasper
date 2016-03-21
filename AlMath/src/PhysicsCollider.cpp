#include <Jasper\PhysicsCollider.h>

namespace Jasper {

using namespace std;

PhysicsCollider::PhysicsCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world)
	:Component(name), m_world(world), m_mesh(mesh)
{

}

PhysicsCollider::PhysicsCollider(const std::string& name, const Vector3& halfExtents, PhysicsWorld* world)
	: Component(name), m_world(world), m_halfExtents(halfExtents)
{
}


PhysicsCollider::~PhysicsCollider()
{
	if (m_collisionShape) {
		delete m_collisionShape;
	}
	if (m_defaultMotionState) {
		delete m_defaultMotionState;
	}
	/*if (m_rigidBody) {
		delete m_rigidBody;
	}*/
}

void PhysicsCollider::Initialize()
{

}

void PhysicsCollider::Destroy()
{
	Component::Destroy();
}

void PhysicsCollider::Awake()
{
	
	Component::Awake();
}

void PhysicsCollider::Start()
{
	Component::Start();
}

void PhysicsCollider::FixedUpdate()
{

}

void PhysicsCollider::Update()
{
	Component::Update();
}

void PhysicsCollider::LateUpdate()
{
}

Transform PhysicsCollider::GetCurrentWorldTransform()
{
	btTransform trans;
	m_rigidBody->getMotionState()->getWorldTransform(trans);
	return Transform(trans);
}



} // namespace Jasper
