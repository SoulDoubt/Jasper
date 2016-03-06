#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Common.h"
#include "vector4.h"
#include "Transform.h"

namespace Jasper {

	ALIGN16
	class DirectionalLight
	{
	public:

		ALIGN_16_OPERATORS;

		DirectionalLight();
		virtual ~DirectionalLight();

		Vector3 Color;

		Vector3 Position() const {
			return transform.Position;
		}

		Vector3 Direction;
		float AmbientIntensity;
		float DiffuseIntensity;

		float ConstAtten;
		float LinearAtten;
		float Exp;

		Transform transform;

		void SetPosition(const Vector3& p) {
			transform.Position = { p.x, p.y, p.z };
		}


	};
}
#endif
