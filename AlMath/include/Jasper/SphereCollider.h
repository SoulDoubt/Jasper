#ifndef _JASPER_SPHERE_COLLIDER_H_
#define _JASPER_SPHERE_COLLIDER_H_

#include "PhysicsCollider.h"

namespace Jasper {

class SphereCollider :
	public PhysicsCollider
{
public:
	SphereCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world);
	~SphereCollider();

	void Awake() override;
};

}

#endif //_JASPER_SPHERE_COLLIDER_H_