#include <Jasper\CharacterController.h>
#include <bullet\btBulletDynamicsCommon.h>

namespace Jasper {

CharacterController::CharacterController(btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight, PhysicsWorld* world, int upAxis)
	: btKinematicCharacterController(ghostObject, convexShape, stepHeight, upAxis)
{
	m_world = world;
}

CharacterController::~CharacterController()
{
}

void CharacterController::debugDraw(btIDebugDraw* debugDrawer)
{
	
}


}