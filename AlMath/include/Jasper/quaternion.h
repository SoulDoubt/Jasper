#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Common.h"
#include "matrix.h"
#include <bullet\btBulletDynamicsCommon.h>

namespace Jasper {

struct Angles {
	float Roll;
	float Pitch;
	float Yaw;

	Angles(float roll, float pitch, float yaw) : Roll(roll), Pitch(pitch), Yaw(yaw) {}
};


class Quaternion
{
public:	

	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3& vector, float scalar);
	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(const btQuaternion& q) : x(q.x()), y(q.y()), z(q.z()), w(q.w()) {}
	Quaternion(const Quaternion& o) = default;
	btQuaternion AsBtQuaternion() { return btQuaternion(x, y, z, w); }

	Matrix3 ToMatrix3() const;
	Matrix4 ToMatrix4() const;

	float operator[](int index) const;
	float& operator[](int index);
	Quaternion operator-() const;
	Quaternion& operator=(const Quaternion& o);
	Quaternion operator+(const Quaternion& o) const;
	Quaternion& operator+=(const Quaternion& o);
	Quaternion operator-(const Quaternion& o) const;
	Quaternion& operator-=(const Quaternion& o);	
	Quaternion& operator/=(const float f);
	Quaternion operator/(const float f) const;
	Quaternion& operator*=(const float f);
	Quaternion operator*(const float f) const;
	Quaternion& operator*=(const Quaternion& o);
	
		
	float Scalar()const { return w; }
	Vector3 xyz() const { return Vector3(x, y, x); }
	void Setxyz(const Vector3& v) { x = v.x, y = v.y, z = v.z; }
	float Pitch() const;
	float Roll() const;
	float Yaw() const;
	Angles ToEulerAngles() const;


	float Length() const;

	float x, y, z, w;

	static Quaternion FromAxisAndAngle(const Vector3& axis, float angle);

	float* AsFloatPtr() {
		return &x;
	}
};

inline Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

inline Quaternion::Quaternion(const Vector3 & vector, float scalar)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = scalar;
}

inline float Quaternion::operator[](int index) const {
	return (&x)[index];
}

inline float& Quaternion::operator[](int index) {
	return (&x)[index];
}

inline Quaternion Quaternion::operator-() const {
	return Quaternion(-x, -y, -z, -w);
}

inline Quaternion& Quaternion::operator=(const Quaternion& o) {
	x = o.x;
	y = o.y;
	z = o.z;
	w = o.w;
	return *this;
}

inline Quaternion Quaternion::operator+(const Quaternion& o) const {
	return Quaternion(x + o.x, y + o.y, z + o.z, w + o.w);
}

inline Quaternion& Quaternion::operator+=(const Quaternion& o) {
	x += o.x;
	y += o.y;
	z += o.z;
	w += o.w;
	return *this;
}

inline Quaternion Quaternion::operator-(const Quaternion& o) const {
	return Quaternion(x - o.x, y - o.y, z - o.z, w - o.w);
}

inline Quaternion& Quaternion::operator-=(const Quaternion& o) {
	x -= o.x;
	y -= o.y;
	z -= o.z;
	w -= o.w;
	return *this;
}

inline Quaternion operator*(const Quaternion& a, const Quaternion& b) {
	Quaternion q;
	q.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	q.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	q.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	q.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	return q;
}

inline Quaternion& Quaternion::operator*=(const Quaternion& o) {
	*this = *this * o;
	return *this;
}

inline Quaternion Normalize(const Quaternion& q) {
	return q * (1.0f / q.Length());
}

inline Quaternion & Quaternion::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= w;
	return *this;
}

inline Quaternion Quaternion::operator/(const float f) const
{
	return Quaternion(x / f, y / f, z / f, w / f);
}

inline Quaternion& Quaternion::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= w;
	return *this;
}

inline Vector3 operator*(const Quaternion& q, const Vector3& v) {
	const Vector3 t = 2.0f * Cross(q.xyz(), v);
	return (v + q.w * t + Cross(q.xyz(), t));
}

inline Vector3 operator*(const Vector3& v, const Quaternion& q) {
	const Vector3 t = 2.0f * Cross(q.xyz(), v);
	return (v + q.w * t + Cross(q.xyz(), t));
}

inline Quaternion Quaternion::operator*(const float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

inline float Dot(const Quaternion& a, const Quaternion& b) {
	return (a.xyz().Dot(b.xyz())) + a.w * b.w;
}

inline Quaternion Conjugate(const Quaternion& q)
{
	Quaternion c(-q.xyz(), q.w);
	return c;
}

inline Quaternion Inverse(const Quaternion& q)
{
	return Conjugate(q) / (Dot(q, q));	
}

inline float Quaternion::Pitch() const
{
	return std::atan2f(2.0f * y * z + w * x,
		w * w - x * x - y * y + z * z);
}

inline float Quaternion::Roll() const
{
	return std::atan2f(2.0f * x * y + z * w,
		x * x + w * w - y * y - z * z);
}

inline float Quaternion::Yaw() const
{
	return std::asinf(-2.0f * (x * z * - w * y));
}

inline Angles Quaternion::ToEulerAngles() const
{
	return Angles(Roll(), Pitch(), Yaw());
}

inline float Quaternion::Length() const
{
	float len = x * x + y * y + z * z + w * w;
	return std::sqrtf(len);
}

inline Quaternion Quaternion::FromAxisAndAngle(const Vector3 & axis, float angle)
{
	Quaternion q;

	const Vector3 a = Normalize(axis);
	const float s = std::sinf(0.5f * angle);
	q.Setxyz(a * s);
	q.w = std::cosf(0.5f * angle);

	return q;	
}



inline Quaternion operator*(float f, const Quaternion& q) {
	return Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
}


}

#endif

