#ifndef _VECTOR_4_H_
#define _VECTOR_4_H_

#include "vector3.h"

namespace Jasper {

class Vector4 {

private:
	bool Compare(const Vector4& o) const;
	bool Compare(const Vector4& o, float epsilon) const;

public:
	float x, y, z, w;

	Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {
	}

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(const Vector3& xyz, float w);

	Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	float Length() const {
		float lenSquared = x * x + y * y + z * z + w * w;
		return sqrtf(lenSquared);
	}

	Vector4& Normalize() {
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
		return *this;
	}

	Vector4 Normalized() const {
		float len = Length();
		return Vector4(x / len, y / len, z / len, w / len);
	}

	Vector4 operator*(const float f) const {
		return Vector4(x * f, y * f, z * f, w * f);
	}

	Vector4& operator*=(const float f) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	Vector4& operator/=(const float f) {
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	Vector4& operator+=(const float f) {
		x += f;
		y += f;
		z += f;
		w += f;
		return *this;
	}

	Vector4& operator-=(const float f) {
		x -= f;
		y -= f;
		z -= f;
		w -= f;
		return *this;
	}

	Vector4& operator+=(const Vector4& o) {
		x += o.x;
		y += o.y;
		z += o.z;
		w += o.w;
		return *this;
	}

	Vector4& operator-=(const Vector4& o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
		w -= o.w;
		return *this;
	}

	Vector4& operator*=(const Vector4& o) {
		x *= o.x;
		y *= o.y;
		z *= o.z;
		w *= o.w;
		return *this;
	}

	bool operator==(const Vector4& o) {
		return Compare(o);
	}

	bool operator !=(const Vector4& o) {
		return !Compare(o);
	}

	Vector3 AsVector3() const {
		return Vector3(x, y, z);
	}

	float Dot(const Vector4& o)const {
		return x * o.x + y * o.y + z * o.z + w * o.w;
	}

	float& operator[](int index) {
		return (&x)[index];
	}

	float operator[](int index) const {
		return (&x)[index];
	}

	std::string ToString() const;

	const float* AsFloatPtr() const {
		return &x;
	}


};

inline bool Vector4::Compare(const Vector4& o) const {
	return ((x == o.x) && (y == o.y) && (z == o.z) && (w == o.w));
}

inline bool Vector4::Compare(const Vector4& o, const float epsilon) const {
	if (fabs(x - o.x) > epsilon) {
		return false;
	}
	if (fabs(y - o.y) > epsilon) {
		return false;
	}
	if (fabs(z - o.z) > epsilon) {
		return false;
	}
	if (fabs(w - o.w) > epsilon) {
		return false;
	}
	return true;
}

inline Vector4::Vector4(const Vector3 & v, float dub)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = dub;
}

inline Vector4 operator*(const Vector4& a, const Vector4& b) {
	return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline std::string Vector4::ToString() const {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

}
#endif
