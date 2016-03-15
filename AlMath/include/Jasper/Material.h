#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vector.h"
#include "Texture.h"
#include "Shader.h"
#include <memory>

namespace Jasper {

class Material
{

private:

	Shader* m_shader;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<CubemapTexture> m_cubemap;

public:
	explicit Material(Shader* shader);
	~Material();
	// onto a material we will attach a shader...
	// a mesh will have a material assigned
	// meshes handle their various geometry buffers

	/* GLSL Format
	struct material{
		vec3 ka;
		vec3 kd;
		vec3 ks;
		float ns;
	};
	*/
	Vector3 Ambient;
	Vector3 Diffuse;
	Vector3 Specular;
	float Shine;

	Texture* GetTexture2D() {
		return m_texture.get();
	}

	void SetTexture2D(std::string filename) {
		auto tex = std::make_unique<Texture>();
		tex->Load(filename);
		m_texture = std::move(tex);
	}

	void SetTexture2D(std::unique_ptr<Texture> tex) {
		m_texture = std::move(tex);
	}

	CubemapTexture* GetCubemapTexture() {
		return m_cubemap.get();
	}

	void SetCubemapTextures(const std::string& posx,
		const std::string& negx, const std::string& posy,
		const std::string& negy, const std::string& posz,
		const std::string& negz) {
		auto cm = std::make_unique<CubemapTexture>();
		cm->Load(posx, negx, posy, negy, posz, negz);
		m_cubemap = std::move(cm);
	}

	Shader* GetShader() {
		return m_shader;
	}

	/*void SetShader(Shader* shader) {
		m_shader = shader;
		m_shader->SetBoundMaterial(this);
	}*/

};
}
#endif // _MATERIAL_H_