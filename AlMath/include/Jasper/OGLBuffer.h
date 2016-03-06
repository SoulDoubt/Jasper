#ifndef _OGL_BUFFER_H_
#define _OGL_BUFFER_H_

namespace Jasper {

class OGLBuffer
{
public:

	enum BufferType {
		VERTEX = 0x8892, // GL_ARRAY_BUFFER
		INDEX = 0x8893 // GL_ELEMENT_ARRAY_BUFFER
	};

	enum UsagePattern
	{
		StreamDraw = 0x88E0, // GL_STREAM_DRAW
		StreamRead = 0x88E1, // GL_STREAM_READ
		StreamCopy = 0x88E2, // GL_STREAM_COPY
		StaticDraw = 0x88E4, // GL_STATIC_DRAW
		StaticRead = 0x88E5, // GL_STATIC_READ
		StaticCopy = 0x88E6, // GL_STATIC_COPY
		DynamicDraw = 0x88E8, // GL_DYNAMIC_DRAW
		DynamicRead = 0x88E9, // GL_DYNAMIC_READ
		DynamicCopy = 0x88EA  // GL_DYNAMIC_COPY
	};

	OGLBuffer();
	explicit OGLBuffer(OGLBuffer::BufferType type);
	~OGLBuffer();

	bool Create();
	void Destroy();
	bool IsCreated() const;

	bool Bind();
	void Release();

	unsigned BufferID() const;

	void SetUsagePattern(UsagePattern usage);

	void Allocate(const void* data, int count);

	void UpdateContents(int offset, int size, const void* data);

	int Size();



private:

	unsigned m_bufferID;
	BufferType m_type;
	UsagePattern m_usage;

};

}
#endif
