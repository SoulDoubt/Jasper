#include <Jasper\LitShader.h>
#include <Jasper\Material.h>

namespace Jasper {

using namespace std;


LitShader::LitShader() : Shader("Lit_Shader")
{
	Initialize();
}


LitShader::~LitShader()
{
}

void LitShader::Initialize()
{
	string vsFile = "./Shaders/lit_vert.glsl";
	string fsFile = "./Shaders/lit_frag.glsl";

	AddShader(vsFile, ShaderType::VERTEX);
	AddShader(fsFile, ShaderType::FRAGMENT);

	LinkShaderProgram();
}

DirectionalLightUniformLocations LitShader::GetDirectionalLightUniformLocations()
{
	DirectionalLightUniformLocations dlul;
	GLuint id = ProgramID();
	dlul.AmbientIntensity = glGetUniformLocation(id, "light0.AmbientIntensity");
	dlul.Color = glGetUniformLocation(id, "light0.Color");
	dlul.ConstAtten = glGetUniformLocation(id, "light0.ConstAtten");
	dlul.DiffuseIntensity = glGetUniformLocation(id, "light0.DiffuseIntensity");
	dlul.Direction = glGetUniformLocation(id, "light0.Direction");
	dlul.Exp = glGetUniformLocation(id, "light0.Exp");
	dlul.LinearAtten = glGetUniformLocation(id, "light0.LinearAtten");
	dlul.Position = glGetUniformLocation(id, "light0.Position");
	return dlul;
}

void LitShader::SetDirectionalLightUniforms(const DirectionalLight* dl, const Vector3& eyeSpacePosition)
{
	auto ul = GetDirectionalLightUniformLocations();
	glUniform3fv(ul.Color, 1, dl->Color.AsFloatPtr());
	glUniform3fv(ul.Direction, 1, dl->Direction.AsFloatPtr());
	glUniform3fv(ul.Position, 1, eyeSpacePosition.AsFloatPtr());
	glUniform1f(ul.AmbientIntensity, dl->AmbientIntensity);
	glUniform1f(ul.DiffuseIntensity, dl->DiffuseIntensity);
	glUniform1f(ul.ConstAtten, dl->ConstAtten);
	glUniform1f(ul.LinearAtten, dl->LinearAtten);
	glUniform1f(ul.Exp, dl->Exp);
}

MaterialUniformLocations LitShader::GetMaterialUniformLocations()
{
	GLuint id = ProgramID();
	MaterialUniformLocations mul;
	mul.Ka = glGetUniformLocation(id, "material0.ka");
	mul.Kd = glGetUniformLocation(id, "material0.kd");
	mul.Ks = glGetUniformLocation(id, "material0.ks");
	mul.Ns = glGetUniformLocation(id, "material0.ns");
	return mul;
}

void LitShader::SetMaterialUniforms(const Material* m)
{
	auto ul = GetMaterialUniformLocations();
	glUniform3fv(ul.Ka, 1, m->Ambient.AsFloatPtr());
	glUniform3fv(ul.Kd, 1, m->Diffuse.AsFloatPtr());
	glUniform3fv(ul.Ks, 1, m->Specular.AsFloatPtr());
	glUniform1fv(ul.Ns, 1, &(m->Shine));
}

void LitShader::SetTransformUniforms(const Transform & trans)
{
	uint id = ProgramID();
	Quaternion q = trans.Orientation;
	uint positionLoc = glGetUniformLocation(id, "transform.position");
	uint orientationLoc = glGetUniformLocation(id, "transform.orientation");
	uint scaleLoc = glGetUniformLocation(id, "transform.scale");

	auto pos = trans.Position;
	auto rot = trans.Orientation;
	auto scale = trans.Scale;

	glUniform3f(positionLoc, pos.x, pos.y, pos.z);
	glUniform4f(orientationLoc, rot.x, rot.y, rot.z, rot.w);
	glUniform3f(scaleLoc, scale.x, scale.y, scale.z);

}

void LitShader::SetPointLightUniforms(const PointLight * pl)
{
	const auto position = pl->GetWorldTransform().Position;
	auto ul = GetDirectionalLightUniformLocations();

	glUniform3fv(ul.Color, 1, pl->Color.AsFloatPtr());
	glUniform3fv(ul.Position, 1, position.AsFloatPtr());
	glUniform1fv(ul.AmbientIntensity, 1, &pl->AmbientIntensity);
	glUniform1fv(ul.ConstAtten, 1, &pl->ConstAtten);
}


} // namespace Jasper