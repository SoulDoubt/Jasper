#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"
#include "Transform.h"
#include <memory>

class btCollisionShape;
class btRigidbody;

namespace Jasper {

class PhysicsWorld;

ALIGN16
class Camera
{
public:

	ALIGN_16_OPERATORS;

	enum class CameraType {
		FIRST_PERSON,
		FLYING
	};

	explicit Camera(CameraType type);
	virtual ~Camera();

	void Awake();

	void SetPhysicsWorld(PhysicsWorld* world);

	Matrix4 GetViewMatrix();
	Matrix4 GetCubemapViewMatrix();
	void Rotate(float pitch, float roll, float yaw);
	void Translate(float x, float y, float z);
	void Translate(const Vector3& vec);

	Vector3 GetPosition() const {		
		return transform.Position;
	}

	Transform transform;

private:

	

	CameraType m_type;

	static Vector3 WORLD_X_AXIS;
	static Vector3 WORLD_Y_AXIS;
	static Vector3 WORLD_Z_AXIS;

	Vector3 m_localXAxis;
	Vector3 m_localYAxis;
	Vector3 m_localZAxis;

	Vector3 m_viewVector;
	Vector3 m_upVector;
	Vector3 m_rightVector;

	float m_accumPitch = 0.f;

	PhysicsWorld* m_physicsWorld = nullptr;

	std::unique_ptr<btCollisionShape> m_collisionShape;
	std::unique_ptr<btRigidBody> m_rigidBody;

};

inline void Camera::SetPhysicsWorld(PhysicsWorld* world) {
	m_physicsWorld = world;
}

}
#endif
