#include <Jasper\Transform.h>

namespace Jasper {

Transform& Transform::Rotate(const Vector3 & axis, float angle)
{
	Orientation = Orientation * Quaternion::FromAxisAndAngle(axis, angle);
	return *this;
}

Transform& Transform::RotateAround(const Vector3& point, const Vector3& axis, const float degrees) {
	auto position = this->Position;
	auto currentRotation = this->Orientation;
	auto rotation = Quaternion::FromAxisAndAngle(axis, DEG_TO_RAD(degrees));
	auto direction = position - point;	
	direction = rotation * direction;
	this->Position = point + direction;
	this->Orientation *= Inverse(currentRotation) * rotation * currentRotation;
	return *this;
}

} // namespace Jasper