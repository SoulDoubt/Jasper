#include <Jasper\PhysicsCollider.h>
#include <Jasper\GameObject.h>

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
	auto go = this->GetGameObject();	
	Transform t = this->GetCurrentWorldTransform();

	go->SetLocalTransform(t);
	//Component::Update();
}

void PhysicsCollider::LateUpdate()
{
}

Transform PhysicsCollider::GetCurrentWorldTransform()
{
	Transform t = GetGameObject()->GetLocalTransform();
	btTransform trans;
	m_rigidBody->getMotionState()->getWorldTransform(trans);
	Transform physTrsnaform = Transform(trans);
	physTrsnaform.Scale = t.Scale;
	return physTrsnaform;
}



} // namespace Jasper
