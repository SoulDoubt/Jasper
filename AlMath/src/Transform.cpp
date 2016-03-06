#include <Jasper\Transform.h>

namespace Jasper {

	Transform& Transform::Rotate(const Vector3 & axis, float angle)
	{
		Orientation = Orientation * Quaternion::FromAxisAndAngle(axis, angle);
		return *this;
	}
}