#include <Jasper\FontRenderer.h>

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <Jasper\stb_truetype.h>
//#include "stb_rect_pack.h"

#include <gl\glew.h>


using namespace std;

namespace Jasper {

	stbtt_bakedchar m_cdata[96];

	FontRenderer::FontRenderer()
		: m_vertexBuffer(GLBuffer::BufferType::VERTEX),
		m_indexBuffer(GLBuffer::BufferType::INDEX)
	{
		m_vertexBuffer.SetUsagePattern(GLBuffer::UsagePattern::DynamicDraw);
		m_indexBuffer.SetUsagePattern(GLBuffer::UsagePattern::DynamicDraw);
		m_shader = make_unique<FontShader>();
	}


	FontRenderer::~FontRenderer()
	{
		m_vertexBuffer.Destroy();
		m_indexBuffer.Destroy();
	}



	void FontRenderer::RenderText(const string& text, float x, float y)
	{
		std::vector<Vertex> verts;
		std::vector<unsigned> indices;
		const char* chars = text.c_str();
		while (*chars) {
			if (*chars >= 32 && *chars < 128) {
				unsigned indexStart = verts.size();
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(m_cdata, 512, 512, *chars - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
				/*glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y0);
				glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y0);
				glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y1);
				glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y1);*/
				Vertex v0 = Vertex(q.x0, q.y0, 0.f, q.s0, q.t1);
				Vertex v1 = Vertex(q.x1, q.y0, 0.f, q.s1, q.t1);
				Vertex v2 = Vertex(q.x1, q.y1, 0.f, q.s1, q.t0);
				Vertex v3 = Vertex(q.x0, q.y1, 0.f, q.s0, q.t0);
				verts.push_back(v0);
				verts.push_back(v1);
				verts.push_back(v2);
				verts.push_back(v3);
				indices.push_back(indexStart + 0);
				indices.push_back(indexStart + 1);
				indices.push_back(indexStart + 2);
				indices.push_back(indexStart + 3);
			}
			++chars;
		}

		m_shader->Bind();
		glBindVertexArray(m_vao);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		m_vertexBuffer.Allocate(verts.data(), verts.size() * sizeof(Vertex));
		m_indexBuffer.Allocate(indices.data(), indices.size() * sizeof(GLuint));
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_shader->Release();
		glBindVertexArray(0);
	}


	void FontRenderer::Initialize()
	{
		m_fontBuffer = (unsigned char*)malloc((1 << 20) * sizeof(unsigned char));
		m_tempBitmap = (unsigned char*)malloc(512 * 512 * sizeof(unsigned char));
		fread(m_fontBuffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
		stbtt_BakeFontBitmap(m_fontBuffer, 0, 32.0, m_tempBitmap, 512, 512, 32, 96, m_cdata);
		free(m_fontBuffer);
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_tempBitmap);
		free(m_tempBitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		int positionLocation = m_shader->PositionAttributeLocation();
		int texLocation = m_shader->TexCoordAttributeLocation();
		m_vertexBuffer.Create();
		m_indexBuffer.Create();
		m_vertexBuffer.Bind();
		m_indexBuffer.Bind();

		size_t positionOffset = offsetof(Vertex, Position);
		size_t texOffset = offsetof(Vertex, TexCoords);

		m_shader->SetAttributeArray(positionLocation, GL_FLOAT, (void*)positionOffset, 3, sizeof(Vertex));
		m_shader->SetAttributeArray(texLocation, GL_FLOAT, (void*)texOffset, 2, sizeof(Vertex));
		m_shader->Release();
		glBindVertexArray(0);

	}
}