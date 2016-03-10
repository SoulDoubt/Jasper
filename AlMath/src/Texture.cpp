#define STB_IMAGE_IMPLEMENTATION
#include <Jasper\stb_image.h>
#include <Jasper\Texture.h>
#include <gl\glew.h>
#include <Jasper\GLError.h>

namespace Jasper {

	using namespace std;

	Texture::Texture()
	{
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureID);
	}

	bool Texture::Load(std::string filename)
	{
		int x, y, comp;

		auto image = stbi_load(filename.c_str(), &x, &y, &comp, STBI_rgb_alpha);
		if (image) {
			printf("Loaded texture from: %s\n", filename.c_str());

			glGenTextures(1, &m_textureID);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			GLERRORCHECK;
			/*int err = glGetError();
			if (err != 0) {
				printf("GL Error in texture load");
			}*/
			stbi_image_free(image);
			return true;
		}
		else {
			printf("Failed to load texture from %s", filename.c_str());
			return false;
		}
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::Release()
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	bool CubemapTexture::Load(const std::string & posx, const std::string & negx, const std::string & posy, const std::string & negy, const std::string & posz, const std::string & negz)
	{
		string files[6] = { posx, negx, posy, negy, posz, negz };
		/*GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };*/
		glGenTextures(1, &m_cubemapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

		for (int i = 0; i < 6; ++i) {
			int x, y, comp;
			auto image = stbi_load(files[i].c_str(), &x, &y, &comp, STBI_rgb_alpha);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			delete image;
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return true;




	}

	CubemapTexture::~CubemapTexture()
	{
		if (m_cubemapID != 0)
			glDeleteTextures(1, &m_cubemapID);
	}

	void CubemapTexture::Bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);
	}

	void CubemapTexture::Release()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

}