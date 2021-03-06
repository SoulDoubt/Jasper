#ifndef _JASPER_PHYSICS_COLLIDER_H_
#define _JASPER_PHYSICS_COLLIDER_H_


#include "Component.h"
#include "PhysicsWorld.h"
#include "Transform.h"



namespace Jasper {

class Mesh;

enum class PHYSICS_COLLIDER_TYPE {
	Box,
	Capsule, 
	ConvexHull,
	Cylinder,
	Sphere
};

class PhysicsCollider :	public Component
{
public:
	explicit PhysicsCollider(const std::string& name, Mesh* mesh, PhysicsWorld* world);
	explicit PhysicsCollider(const std::string& name, const Vector3& halfExtents, PhysicsWorld* world);
	virtual ~PhysicsCollider();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	Transform GetCurrentWorldTransform();

	PhysicsWorld* GetPhysicsWorld() { return m_world; }
	btRigidBody* GetRigidBody() { return m_rigidBody; }
	btCollisionShape* GetCollisionShape() { return m_collisionShape; }

	btTransform GetDebugTransform() {
		btTransform trans;
		m_rigidBody->getMotionState()->getWorldTransform(trans);
		return trans;
	}

	void SetWorldTransform(const btTransform& trans) {
		m_rigidBody->getMotionState()->setWorldTransform(trans);
	}

	float Mass = 0.f;
	float Restitution = 0.5f;
	float Friction = 0.75f;

protected:
	PhysicsWorld* m_world;
	Mesh* m_mesh;
	btCollisionShape* m_collisionShape;
	btDefaultMotionState* m_defaultMotionState;
	btRigidBody* m_rigidBody;
	Vector3 m_halfExtents;

	
};

} // namespace Jasper
#endif _PHYSICS_COLLIDER_H_