#include <Jasper\CharacterController.h>

namespace Jasper {



CharacterController::CharacterController(PhysicsWorld * world)
	:btKinematicCharacterController(m_ghost, m_shape, m_stepHeight)
{
}

CharacterController::~CharacterController()
{
}


}