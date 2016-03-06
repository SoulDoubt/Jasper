#ifndef _JASPER_FONT_RENDERER_H
#define _JASPER_FONT_RENDERER_H


#include "MeshRenderer.h"
#include "OGLBuffer.h"
#include "matrix.h"
#include "FontShader.h"

namespace Jasper {

class FontRenderer
{
public:

	FontRenderer();
	~FontRenderer();

	void RenderText(const std::string& text, float x, float y);
	void Initialize();

private:

	unsigned char* m_fontBuffer = nullptr;// [1 << 20];
	unsigned char*  m_tempBitmap = nullptr; //[512 * 512];

	unsigned m_texID;

	Matrix4 m_projectionMatrix;
	Matrix4 m_viewMatrix;

	OGLBuffer m_vertexBuffer;
	OGLBuffer m_indexBuffer;

	std::unique_ptr<FontShader> m_shader;

	unsigned int m_vao;


};
}

#endif // _JASPER_FONT_RENDERER_H