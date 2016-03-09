#ifndef _JASPER_LIT_SHADER_H_
#define _JASPER_LIT_SHADER_H_

#include "Shader.h"
#include "Transform.h"

namespace Jasper {

class LitShader :
	public Shader
{
public:
	LitShader();
	~LitShader();

	void Initialize() override;
	virtual DirectionalLightUniformLocations GetDirectionalLightUniformLocations() override;
	virtual void SetLightUniforms(const DirectionalLight* dl, const Vector3& eyeSpacePosition) override;

	virtual MaterialUniformLocations GetMaterialUniformLocations()override;
	virtual void SetMaterialUniforms(const Material* m) override;

	void SetTransformUniforms(const Transform& trans) override;
};
}
#endif //_LIT_SHADER_H_