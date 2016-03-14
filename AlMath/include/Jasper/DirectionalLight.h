#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Common.h"
#include "vector.h"
#include <Jasper\GameObject.h>

namespace Jasper {

class DirectionalLight : public GameObject
{
public:

	DirectionalLight(const std::string& name);
	virtual ~DirectionalLight();

	Vector3 Color;	

	Vector3 Direction;
	float AmbientIntensity;
	float DiffuseIntensity;

	float ConstAtten;
	float LinearAtten;
	float Exp;	
	
};

}
#endif
