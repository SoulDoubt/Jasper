#include <Jasper\Shader.h>
#include <Jasper\Material.h>
#include <Jasper\GLError.h>

namespace Jasper {
	using namespace std;

	Shader::Shader() {
		Initialize();
	}

	Shader::Shader(std::string name) {
		m_name = name;
		Initialize();
	}


	Shader::~Shader() {
		Destroy();
	}

	void Shader::Initialize() {
		m_programID = 0;
		m_programID = glCreateProgram();
	}

	bool Shader::Bind() {
		if (!m_programID) {
			return false;
		}
		glUseProgram(m_programID);
		return true;
	}

	void Shader::Release() {
		glUseProgram(0);
	}

	int Shader::GetAttributeLocation(const char* name) {
		return glGetAttribLocation(m_programID, name);
	}

	void Shader::SetAttributeArray(const char* name, GLenum type, const void* values, int tupleSize, int stride) {
		int location = GetAttributeLocation(name);
		if (location > -1) {
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, tupleSize, type, GL_TRUE, stride, values);
		}
	}

	void Shader::SetAttributeArray(int location, GLenum num_type, void * offset, int num_components, int stride)
	{
		if (location > -1) {
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, num_components, num_type, GL_FALSE, stride, offset);
		}
	}

	void Shader::AddShader(std::string filename, ShaderType t) {

		FILE* file;
		auto err = fopen_s(&file, filename.c_str(), "r");
		if (file && err == 0) {
			char* file_contents;
			long file_size;
			fseek(file, 0, SEEK_END);
			file_size = ftell(file);
			rewind(file);
			file_contents = (char*)calloc((file_size + 1), sizeof(char));
			fread(file_contents, sizeof(char), file_size, file);
			fclose(file);
			file_contents[file_size] = 0; // null terminate the string
										  //printf(file_contents);
			const char* source = file_contents;
			//printf_s("%s\n,", c_source);
			std::string logType = "";
			GLenum st = GL_VERTEX_SHADER;
			switch (t) {
			case VERTEX:
				st = GL_VERTEX_SHADER;
				logType = "VERTEX";
				break;
			case FRAGMENT:
				st = GL_FRAGMENT_SHADER;
				logType = "FRAGMENT";
				break;
			case GEOMETRY:
				st = GL_GEOMETRY_SHADER;
				logType = "GEOMETRY";
			}

			GLuint shaderID = glCreateShader(st);
			glShaderSource(shaderID, 1, &source, NULL);
			free(file_contents);
			glCompileShader(shaderID);

			GLint status;
			std::string info;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) {
				GLint logLength;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
				GLchar* log = new GLchar[logLength + 1];
				glGetShaderInfoLog(shaderID, logLength, NULL, log);
				printf_s("%s\n", log);
				delete[] log;
			}
			printf_s("%s: %s shader compiled successfully\n", m_name.c_str(), logType.c_str());
			m_shaders.push_back(shaderID);
		}

	}

	void Shader::LinkShaderProgram() {
		if (m_programID < 1) {
			throw std::exception("No Shader Program created while linking shaders...\n");
		}
		for (auto shader : m_shaders) {
			glAttachShader(m_programID, shader);
		}

		glLinkProgram(m_programID);
		GLint status;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength = 256;
			//glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(m_programID, infoLogLength, NULL, strInfoLog);
			printf_s("Linker failure: %s\n", strInfoLog);
			delete[] strInfoLog;
		}
		else {
			printf_s("Shader Program linked successfully.\n");
		}


		for (auto shader : m_shaders) {
			glDetachShader(m_programID, shader);
			glDeleteShader(shader);
		}
	}

	void Shader::Destroy() {
		for (auto sh : m_shaders) {
			glDetachShader(m_programID, sh);
			glDeleteShader(sh);
		}
		if (m_programID > 0)
			glDeleteProgram(m_programID);
	}


	unsigned Shader::PositionAttributeLocation()
	{
		if (m_positionsAttribute > 0) {
			return m_positionsAttribute;
		}
		else {
			m_positionsAttribute = glGetAttribLocation(m_programID, "position");
			return m_positionsAttribute;
		}
	}

	unsigned Shader::NormalAttributeLocation()
	{
		if (m_normalsAttribute > 0) {
			return m_normalsAttribute;
		}
		else {
			m_normalsAttribute = glGetAttribLocation(m_programID, "normal");
			return m_normalsAttribute;
		}
	}

	unsigned Shader::TexCoordAttributeLocation()
	{
		if (m_texCoordsAttribute > 0) {
			return m_texCoordsAttribute;
		}
		else {
			m_texCoordsAttribute = glGetAttribLocation(ProgramID(), "texCoords");
			return m_texCoordsAttribute;
		}
	}

	uint Shader::ColorsAttributeLocation()
	{
		if (m_colorsAttribute > 0) {
			return m_colorsAttribute;
		}
		else {
			m_colorsAttribute = glGetAttribLocation(ProgramID(), "color");
			return m_colorsAttribute;
		}
	}

	void Shader::SetModelViewMatrix(const Matrix4 & mvm)
	{
		int loc = glGetUniformLocation(m_programID, "mvMatrix");
		glUniformMatrix4fv(loc, 1, GL_TRUE, mvm.AsFloatPtr());
		GLERRORCHECK;
	}

	void Shader::SetModelViewProjectionMatrix(const Matrix4 & mvp)
	{
		int loc = glGetUniformLocation(m_programID, "mvpMatrix");
		glUniformMatrix4fv(loc, 1, GL_TRUE, mvp.AsFloatPtr());
		GLERRORCHECK;
	}

	void Shader::SetNormalMatrix(const Matrix3 & normal)
	{
		int loc = glGetUniformLocation(m_programID, "normalMatrix");
		glUniformMatrix3fv(loc, 1, GL_TRUE, normal.AsFloatPtr());
		GLERRORCHECK;
	}

	DirectionalLightUniformLocations Shader::GetDirectionalLightUniformLocations()
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

	void Shader::SetLightUniforms(DirectionalLight & dl)
	{
		auto ul = GetDirectionalLightUniformLocations();
		glUniform3fv(ul.Color, 1, dl.Color.AsFloatPtr());
		glUniform3fv(ul.Direction, 1, dl.Direction.AsFloatPtr());
		glUniform3fv(ul.Position, 1, dl.Position().AsFloatPtr());
		glUniform1fv(ul.AmbientIntensity, 1, &dl.AmbientIntensity);
		glUniform1fv(ul.DiffuseIntensity, 1, &dl.DiffuseIntensity);
		glUniform1fv(ul.ConstAtten, 1, &dl.ConstAtten);
		glUniform1fv(ul.LinearAtten, 1, &dl.LinearAtten);
		glUniform1fv(ul.Exp, 1, &dl.Exp);
	}

	MaterialUniformLocations Shader::GetMaterialUniformLocations()
	{
		GLuint id = ProgramID();
		MaterialUniformLocations mul;
		mul.Ka = glGetUniformLocation(id, "material0.ka");
		mul.Kd = glGetUniformLocation(id, "material0.kd");
		mul.Ks = glGetUniformLocation(id, "material0.ks");
		mul.Ns = glGetUniformLocation(id, "material0.ns");
		return mul;
	}

	void Shader::SetMaterialUniforms(const Material* m)
	{
		auto ul = GetMaterialUniformLocations();
		glUniform3fv(ul.Ka, 1, m->Ambient.AsFloatPtr());
		glUniform3fv(ul.Kd, 1, m->Diffuse.AsFloatPtr());
		glUniform3fv(ul.Ks, 1, m->Specular.AsFloatPtr());
		glUniform1fv(ul.Ns, 1, &(m->Shine));
	}

	void Shader::SetCameraPosition(const Vector3& cp)
	{
		GLuint loc = glGetUniformLocation(ProgramID(), "cameraPosition");
		glUniform3fv(loc, 1, cp.AsFloatPtr());
	}

	void Shader::SetTransformUniforms(const Transform & trans)
	{
		uint id = ProgramID();
		Quaternion q = trans.Orientation;
		uint positionLoc = glGetUniformLocation(id, "transform.position");
		uint orientationLoc = glGetUniformLocation(id, "transform.orientation");
		uint scaleLoc = glGetUniformLocation(id, "transform.scale");

		glUniform3fv(positionLoc, 1, trans.Position.AsFloatPtr());
		glUniform4fv(orientationLoc, 1, q.AsFloatPtr());
		glUniform3fv(scaleLoc, 1, trans.Scale.AsFloatPtr());

	}
}