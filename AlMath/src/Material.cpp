#include <Jasper\Material.h>

namespace Jasper {

	Material::Material(Shader* shader)
	{
		m_shader = shader;
		m_shader->SetBoundMaterial(this);
		// material defaults
		Ambient = { 0.5f, 0.5f, 0.5f };
		Diffuse = { 0.85f, 0.85f, 0.85f };
		Specular = { 0.25f, 0.25f, 0.26f };
		Shine = 32.0f;
	}


	Material::~Material()
	{
	}
}