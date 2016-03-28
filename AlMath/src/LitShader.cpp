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

void LitShader::GetDirectionalLightUniformLocations()
{
	if (m_dlus.isPopulated) {
		return;
	}
	else {
		GLuint id = ProgramID();		
		m_dlus.Color = glGetUniformLocation(id, "light0.Color");				
		m_dlus.Direction = glGetUniformLocation(id, "light0.Direction");
		m_dlus.isPopulated = true;
	}
}

void LitShader::SetDirectionalLightUniforms(const DirectionalLight* dl)
{
	if (!m_dlus.isPopulated) {
		GetDirectionalLightUniformLocations();
	}
	glUniform3fv(m_dlus.Color, 1, dl->Color.AsFloatPtr());
	glUniform3fv(m_dlus.Direction, 1, dl->Direction.AsFloatPtr());
}

void LitShader::GetMaterialUniformLocations()
{
	if (m_mus.isPopulated) {
		return;
	}
	else {
		GLuint id = ProgramID();		
		m_mus.Ka = glGetUniformLocation(id, "material0.ka");
		m_mus.Kd = glGetUniformLocation(id, "material0.kd");
		m_mus.Ks = glGetUniformLocation(id, "material0.ks");
		m_mus.Ns = glGetUniformLocation(id, "material0.ns");
		m_mus.isPopulated = true;
	}
}

void LitShader::SetMaterialUniforms(const Material* m)
{	
	if (!m_mus.isPopulated) {
		GetMaterialUniformLocations();
	}
	glUniform3fv(m_mus.Ka, 1, m->Ambient.AsFloatPtr());
	glUniform3fv(m_mus.Kd, 1, m->Diffuse.AsFloatPtr());
	glUniform3fv(m_mus.Ks, 1, m->Specular.AsFloatPtr());
	glUniform1fv(m_mus.Ns, 1, &(m->Shine));
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

void LitShader::GetPointLightUniformLocations()
{
	if (m_plus.isPopulated) {
		return;
	}
	else {
		uint id = ProgramID();
		m_plus.Color = glGetUniformLocation(id, "light0.Color");
		m_plus.Position = glGetUniformLocation(id, "light0.Position");
		m_plus.ConstAttenuation = glGetUniformLocation(id, "Light0.ConstAtten");
		m_plus.isPopulated = true;
	}
}

void LitShader::SetPointLightUniforms(const PointLight* pl, const Vector3& eslp)
{			
	if (!m_plus.isPopulated) {
		GetPointLightUniformLocations();
	}
	glUniform3fv(m_plus.Color, 1, pl->Color.AsFloatPtr());
	glUniform3fv(m_plus.Position, 1, eslp.AsFloatPtr());	
	glUniform1fv(m_plus.ConstAttenuation, 1, &pl->ConstAtten);
}


} // namespace Jasper