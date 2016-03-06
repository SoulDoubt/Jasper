#include <Jasper\PhysicsWorld.h>

namespace Jasper {

	PhysicsWorld::PhysicsWorld()
	{
	}


	PhysicsWorld::~PhysicsWorld()
	{
		Destroy();
	}

	void PhysicsWorld::Initialize()
	{
		m_broadphase = new btDbvtBroadphase();
		m_collisionConfig = new btDefaultCollisionConfiguration();
		m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfig);
		m_solver = new btSequentialImpulseConstraintSolver();
		m_world = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfig);
		m_world->setGravity({ 0.0f, -9.81f, 0.0f });
	}

	void PhysicsWorld::Destroy()
	{
		if (m_world)
			delete m_world;
		if (m_solver)
			delete m_solver;
		if (m_collisionDispatcher)
			delete m_collisionDispatcher;
		if (m_collisionConfig)
			delete m_collisionConfig;
		if (m_broadphase)
			delete m_broadphase;

	}

	void PhysicsWorld::Update(float dt)
	{
		m_world->stepSimulation(dt, 10);
	}

	void PhysicsWorld::AddRigidBody(btRigidBody * rb)
	{
		m_world->addRigidBody(rb);
	}
}