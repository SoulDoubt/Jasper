#ifndef _JASPER_PHYSICS_WORLD_H_
#define _JASPER_PHYSICS_WORLD_H_

#include "Common.h"
#include <btBulletDynamicsCommon.h>

namespace Jasper {

ALIGN16
class PhysicsWorld
{
public:

	ALIGN_16_OPERATORS;

	PhysicsWorld();
	~PhysicsWorld();

	PhysicsWorld(const PhysicsWorld& o) = delete;
	PhysicsWorld& operator=(const PhysicsWorld& o) = delete;

	void Initialize();
	void Destroy();
	void Update(float dt);

	void AddRigidBody(btRigidBody* rb);
	void ConvexSweepTest(btConvexShape* shape, btTransform& from, btTransform& to, btCollisionWorld::ClosestConvexResultCallback& callback);

private:

	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfig;
	btCollisionDispatcher* m_collisionDispatcher;
	btSequentialImpulseConstraintSolver* m_solver;

	btDiscreteDynamicsWorld* m_world;

};

inline void PhysicsWorld::ConvexSweepTest(btConvexShape* shape, btTransform& from, btTransform& to, btCollisionWorld::ClosestConvexResultCallback& callback)
{
	m_world->convexSweepTest(shape, from, to, callback);
}

}

#endif // _JASPER_PHYSICS_WORLD_H_