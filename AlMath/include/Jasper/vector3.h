#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include "Common.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <cmath>
#endif
#include <string>

#include <btBulletDynamicsCommon.h>

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(theta) (((float)(M_PI / 180.0f)) * theta)
#endif

namespace Jasper {

struct Vector2 {
	float x, y;

	Vector2(float x, float y);
	Vector2() = default;
};

inline Vector2::Vector2(float x, float y) : x(x), y(y) {
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

inline Vector2 operator+(const Vector2& a, const Vector2& b) {
	return Vector2(a.x + b.x, a.y + b.y);
}

inline float Length(const Vector2& vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y);

}

inline Vector2 Normalize(const Vector2& vec) {
	float len = Length(vec);
	return Vector2(vec.x / len, vec.y / len);
}


ALIGN16
class Vector3
{
public:

	ALIGN_16_OPERATORS

	float x, y, z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const btVector3& v) : x(v.x()), y(v.y()), z(v.z()) {}

	btVector3 AsBtVector3() { return btVector3(x, y, z); }
	
	float Length() const;
	float LengthSquared() const;
	void Normalize();
	Vector3 Normalized() const;

	Vector3& operator=(const Vector3& o);

	Vector3 operator*(const float f) const;
	Vector3 operator/(const float f) const;

	//Vector3 operator+(const Vector3& o) const;
	//ctor3 operator-(const Vector3& o) const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& o);
	Vector3& operator-=(const Vector3& o);
	Vector3& operator*=(const float f);
	Vector3& operator/=(const Vector3& o);
	Vector3& operator/=(const float f);

	bool operator==(const Vector3& o) const;
	bool operator!=(const Vector3& o) const;

	bool Compare(const Vector3& o) const;
	bool Compare(const Vector3& o, float epsilon) const;

	Vector3 Cross(const Vector3& o) const;
	float Dot(const Vector3& o) const;

	std::string ToString(int precision = 4) const;

	float& operator[](int index) {
		return (&x)[index];
	}

	float operator[](int index) const {
		return (&x)[index];
	}

	const float* AsFloatPtr() const {
		return &x;
	}

};

inline float Vector3::Length() const {
	return sqrtf(x * x + y * y + z * z);
}

inline float Vector3::LengthSquared() const {
	return x * x + y * y + z * z;
}

inline float Length(const Vector3& a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

//inline Vector3 Vector3::operator+(const Vector3& o) const {
//	return Vector3(x + o.x, y + o.y, z + o.z);
//}

//inline Vector3 Vector3::operator-(const Vector3& o) const {
//	return Vector3(x - o.x, y - o.y, z - o.z);
//}

inline Vector3 Vector3::operator*(const float f) const {
	return Vector3(x * f, y * f, z * f);
}

inline Vector3 operator*(const float f, const Vector3& v) {
	return v * f;
}

inline Vector3 Vector3::operator/(const float f) const {
	return Vector3(x / f, y / f, z / f);
}

inline Vector3& Vector3::operator+=(const Vector3& o) {
	x += o.x;
	y += o.y;
	z += o.z;
	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& o) {
	x -= o.x;
	y -= o.y;
	z -= o.z;
	return *this;
}

inline void Vector3::Normalize() {
	float len = this->Length();
	x /= len;
	y /= len;
	z /= len;
}

inline Vector3 Vector3::Normalized() const {
	float len = Length();
	return Vector3(x / len, y / len, z / len);
}

inline Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	return Vector3(v.x / len, v.y / len, v.z / len);
}

inline Vector3& Vector3::operator*=(const float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& o) {
	x /= o.x;
	y /= o.y;
	z /= o.z;
	return *this;
}

inline Vector3& Vector3::operator/=(const float f) {
	float invf = 1.0f / f;
	x *= invf;
	y *= invf;
	z *= invf;
	return *this;
}

inline bool Vector3::Compare(const Vector3& o) const {
	return ((x == o.x) && (y == o.y) && (z == o.z));
}

inline bool Vector3::Compare(const Vector3& o, float epsilon) const {
	if (fabs(x - o.x) > epsilon) {
		return false;
	}
	if (fabs(y - o.y) > epsilon) {
		return false;
	}
	if (fabs(z - o.z) > epsilon) {
		return false;
	}
	return true;
}

inline Vector3& Vector3::operator=(const Vector3& o) {
	x = o.x;
	y = o.y;
	z = o.z;
	return *this;
}

inline Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

inline bool Vector3::operator==(const Vector3& o) const {
	return Compare(o);
}

inline bool Vector3::operator!=(const Vector3& o) const {
	return !Compare(o);
}

inline Vector3 Vector3::Cross(const Vector3& o) const {
	return Vector3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
}

inline Vector3 Cross(const Vector3& a, const Vector3& b) {
	float x = a.y * b.z - a.z * b.y;
	float y = a.z * b.x - a.x * b.z;
	float z = a.x * b.y - a.y * b.x;
	return Vector3(x, y, z);
}

inline Vector3 operator+(const Vector3& a, const Vector3& b) {
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator-(const Vector3& a, const Vector3& b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator*(const Vector3& a, const Vector3& b) {
	return{ a.x * b.x, a.y * b.y, a.z * b.z };
	
}

inline float Vector3::Dot(const Vector3& o) const {
	return x * o.x + y * o.y + z * o.z;
}


inline float Dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline std::string Vector3::ToString(int precision) const {
	std::string xval = std::to_string(x);
	std::string yval = std::to_string(y);
	std::string zval = std::to_string(z);
	return "(" + xval + "," + yval + "," + zval + ")";
}

}

#endif