#ifndef _JASPER_CAPSULE_COLLIDER_H_
#define _JASPER_CAPSULE_COLLIDER_H_

#include <Jasper\PhysicsCollider.h>

namespace Jasper {

class CapsuleCollider : public PhysicsCollider {

public:

	CapsuleCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world);
	~CapsuleCollider();

	void Awake() override;
};

} // namespace Jasper

#endif // _JASPER_CAPSULE_COLLIDER_H_
