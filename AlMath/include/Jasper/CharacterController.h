#ifndef _JASPER_CHARACTER_CONTROLLER_H_
#define _JASPER_CHARACTER_CONTROLLER_H_

#include "GameObject.h"
#include <bullet\BulletDynamics\Character\btKinematicCharacterController.h>



namespace Jasper {

class PhysicsWorld;

class CharacterController : public btKinematicCharacterController
{
public:
	explicit CharacterController(PhysicsWorld* world);
	~CharacterController();

	int StepHeight = 1;

private:
	btPairCachingGhostObject* m_ghost;
	PhysicsWorld* m_world;
	btCapsuleShape* m_shape;


};

} // namespace Jasper

#endif // _JASPER_CHARACTER_CONTROLLER_H_