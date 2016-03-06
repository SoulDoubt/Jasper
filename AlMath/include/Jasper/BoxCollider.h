#ifndef _JASPER_BOX_COLLIDER_H_
#define _JASPER_BOX_COLLIDER_H_

#include "PhysicsCollider.h"

namespace Jasper {

class BoxCollider :
	public PhysicsCollider
{
public:
	BoxCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world);
	~BoxCollider();

	void Awake() override;
};

}

# endif // _JASPER_PHYSICS_COLLIDER_H_