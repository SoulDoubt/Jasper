#ifndef _JASPER_SHADER_H_
#define _JASPER_SHADER_H_

#include "Common.h"
#include "matrix.h"
#include <gl\glew.h>
#include <vector>
#include "DirectionalLight.h"
#include "PointLight.h"

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
	virtual int TangentAttributeLocation();

	virtual void SetModelViewMatrix(const Matrix4& mvm);
	virtual void SetModelViewProjectionMatrix(const Matrix4& mvp);
	virtual void SetNormalMatrix(const Matrix3& normal);
	virtual void SetModelMatrix(const Matrix4& model);

	virtual DirectionalLightUniformLocations GetDirectionalLightUniformLocations();
	virtual void SetDirectionalLightUniforms(const DirectionalLight* dl, const Vector3& eyeSpacePosition);
	
	virtual void SetPointLightUniforms(const PointLight* dl);

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
	int m_tangentAttribute = -1;
	Material* m_material;
	std::string m_name;

};

inline const uint Shader::ProgramID() const {
	return m_programID;
}

inline void Shader::SetModelMatrix(const Matrix4& model) {
	int location = glGetUniformLocation(m_programID, "modelMatrix");
	if (location > -1)
		glUniformMatrix4fv(location, 1, GL_TRUE, model.AsFloatPtr());
}

inline int Shader::TangentAttributeLocation() {
	if (m_tangentAttribute > 0) {
		return m_tangentAttribute;
	}
	m_tangentAttribute = glGetAttribLocation(m_programID, "tangent");
}

}

#endif // _SHADER_H_

