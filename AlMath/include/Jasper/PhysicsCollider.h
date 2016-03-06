#ifndef _JASPER_PHYSICS_COLLIDER_H_
#define _JASPER_PHYSICS_COLLIDER_H_


#include "Component.h"
#include "PhysicsWorld.h"
#include "Transform.h"



namespace Jasper {

class Mesh;

class PhysicsCollider :
	public Component
{
public:
	explicit PhysicsCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world);
	virtual ~PhysicsCollider();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	Transform GetCurrentWorldTransform();

	float Mass = 0.f;

protected:
	PhysicsWorld* m_world;
	Mesh* m_mesh;
	btCollisionShape* m_collisionShape;
	btDefaultMotionState* m_defaultMotionState;
	btRigidBody* m_rigidBody;
};
}
#endif _PHYSICS_COLLIDER_H_