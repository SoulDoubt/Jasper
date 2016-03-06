#ifndef _JASPER_SHADER_H_
#define _JASPER_SHADER_H_

#include "Common.h"
#include "matrix.h"
#include <gl\glew.h>
#include <vector>
#include "DirectionalLight.h"

/*

*/
namespace Jasper {

class Material;

struct DirectionalLightUniformLocations {
	uint Color;
	uint Position;
	uint Direction;
	uint AmbientIntensity;
	uint DiffuseIntensity;
	uint ConstAtten;
	uint LinearAtten;
	uint Exp;
};


struct MaterialUniformLocations {
	uint Ka;
	uint Kd;
	uint Ks;
	uint Ns;
};

class Shader
{
public:
	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	Shader();
	Shader(std::string name);
	~Shader();

	void AddShader(std::string filename, ShaderType t);
	void LinkShaderProgram();

	const GLuint ProgramID() const;

	bool Bind();
	void Release();

	int GetAttributeLocation(const char* name);
	void SetAttributeArray(const char* name, GLenum type, const void* values, int tupleSize, int stride);
	void SetAttributeArray(int location, GLenum num_type, void* offset, int num_components, int stride);

	virtual void Initialize();
	virtual void Destroy();

	virtual uint PositionAttributeLocation();
	virtual uint NormalAttributeLocation();
	virtual uint TexCoordAttributeLocation();
	virtual uint ColorsAttributeLocation();


	virtual void SetModelViewMatrix(const Matrix4& mvm);
	virtual void SetModelViewProjectionMatrix(const Matrix4& mvp);
	virtual void SetNormalMatrix(const Matrix3& normal);

	virtual DirectionalLightUniformLocations GetDirectionalLightUniformLocations();
	virtual void SetLightUniforms(DirectionalLight& dl);

	virtual MaterialUniformLocations GetMaterialUniformLocations();
	virtual void SetMaterialUniforms(const Material* m);

	virtual void SetTransformUniforms(const Transform & trans);

	void SetBoundMaterial(Material* m) {
		m_material = m;
	}

	virtual void SetCameraPosition(const Vector3& cp);



private:
	NON_COPYABLE(Shader);
	uint m_programID;
	std::vector<uint> m_shaders;

protected:

	uint m_positionsAttribute = 0;
	uint m_normalsAttribute = 0;
	uint m_texCoordsAttribute = 0;
	uint m_colorsAttribute = 0;
	Material* m_material;
	std::string m_name;

};

inline const uint Shader::ProgramID() const {
	return m_programID;
}

}

#endif // _SHADER_H_
