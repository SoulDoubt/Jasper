#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "GameObject.h"

namespace Jasper {

class PointLight : public GameObject
{
public:
	
	explicit PointLight(const std::string& name);
	~PointLight();

	Vector3 Color = {1.0f, 1.0f, 1.0f};
	float AmbientIntensity = 0.025f;
	float ConstAtten = 0.01f;	
};



} // namespace Jasper

#endif // _POINT_LIGHT_H_