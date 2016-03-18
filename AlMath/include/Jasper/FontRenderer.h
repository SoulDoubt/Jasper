#ifndef _JASPER_FONT_RENDERER_H
#define _JASPER_FONT_RENDERER_H

#include "stb_truetype.h"
#include "MeshRenderer.h"
#include "GLBuffer.h"
#include "matrix.h"
#include "FontShader.h"

namespace Jasper {

class FontRenderer
{
public:

	FontRenderer();
	~FontRenderer();

	void RenderText(const std::string& text, float x, float y);
	std::unique_ptr<Texture> GetTextureAtlas();
	void Initialize();
	void SetOrthoMatrix(const Matrix4& ortho) {
		m_matrix = ortho;
	}

private:	

	unsigned m_texID;



	Matrix4 m_matrix;
	

	GLBuffer m_vertexBuffer;
	GLBuffer m_indexBuffer;

	std::unique_ptr<FontShader> m_shader;
	std::unique_ptr<Texture> m_texture;

	unsigned int m_vao;

	stbtt_bakedchar m_cdata[256];
	stbtt_packedchar m_packData[256];
};

}

#endif // _JASPER_FONT_RENDERER_H