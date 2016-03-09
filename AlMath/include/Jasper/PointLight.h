#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "GameObject.h"

namespace Jasper {

class PointLight : public GameObject
{
public:
	PointLight();
	PointLight(const std::string& name);
	~PointLight();
};
}
#endif // _POINT_LIGHT_H_