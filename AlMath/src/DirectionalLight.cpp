#include <Jasper\DirectionalLight.h>

namespace Jasper {


	DirectionalLight::DirectionalLight()
	{
		Color = { 1.0f, 1.0f, 1.0f };
		Direction = { 0.0f, -1.0f, 0.0f };
		AmbientIntensity = 0.6f;
		DiffuseIntensity = 0.85f;

		ConstAtten = 0.25f;
		LinearAtten = 0.25f;
		Exp = 0.75f;
		SetPosition({ 0.0f, 10.0f, 0.0f });


	}


	DirectionalLight::~DirectionalLight()
	{
	}
}