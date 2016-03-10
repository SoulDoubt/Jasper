#include <Jasper\DirectionalLight.h>

namespace Jasper {


DirectionalLight::DirectionalLight(const std::string& name) : GameObject(name)
{
	Color = { 1.0f, 1.0f, 1.0f };
	Direction = { 0.0f, -1.0f, 0.0f };
	AmbientIntensity = 0.0f;
	DiffuseIntensity = 0.664f;

	ConstAtten = 0.25f;
	LinearAtten = 0.25f;
	Exp = 0.75f;

}


DirectionalLight::~DirectionalLight()
{
}

} // namespace Jasper
