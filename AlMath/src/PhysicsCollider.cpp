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
	Destroy();
}

void PhysicsCollider::Initialize()
{

}

void PhysicsCollider::Destroy()
{	
			
}

void PhysicsCollider::Awake()
{
	
}

void PhysicsCollider::Start()
{
	//Component::Start();
}

void PhysicsCollider::FixedUpdate()
{

}

void PhysicsCollider::Update()
{
	//Component::Update();
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
