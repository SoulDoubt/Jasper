#include <Jasper\OGLBuffer.h>
#include <cassert>
#include <gl\glew.h>

namespace Jasper {

	OGLBuffer::OGLBuffer() : OGLBuffer(BufferType::VERTEX)
	{
	}

	OGLBuffer::OGLBuffer(OGLBuffer::BufferType type) : m_type(type), m_usage(UsagePattern::StaticDraw)
	{
	}


	OGLBuffer::~OGLBuffer()
	{
		if (IsCreated()) {
			Destroy();
		}
	}

	bool OGLBuffer::Create()
	{
		glGenBuffers(1, &m_bufferID);
		if (m_bufferID > 0) {
			return true;
		}
		return false;
	}

	void OGLBuffer::Destroy()
	{
		assert(m_bufferID != 0);
		if (m_bufferID > 0) {
			glDeleteBuffers(1, &m_bufferID);
		}
	}

	bool OGLBuffer::IsCreated() const
	{
		return m_bufferID != 0;
	}

	bool OGLBuffer::Bind()
	{
		assert(m_bufferID != 0);
		if (m_bufferID > 0) {
			glBindBuffer(m_type, m_bufferID);
			return true;
		}
		return false;
	}

	void OGLBuffer::Release()
	{
		assert(m_bufferID != 0);
		if (m_bufferID > 0) {
			glBindBuffer(m_type, 0);
		}
	}

	GLuint OGLBuffer::BufferID() const
	{
		return m_bufferID;
	}

	void OGLBuffer::SetUsagePattern(UsagePattern usage)
	{
		m_usage = usage;
	}

	void OGLBuffer::Allocate(const void * data, int count)
	{
		assert(m_bufferID != 0);
		if (m_bufferID > 0) {
			glBufferData(m_type, count, data, m_usage);
		}

	}

	void OGLBuffer::UpdateContents(int offset, int size, const void* data)
	{
		assert(m_bufferID != 0);
		if (m_bufferID > 0) {

			glBufferSubData(m_type, offset, size, data);

		}

	}

	int OGLBuffer::Size()
	{
		assert(m_bufferID > 0);
		int sz = -1;
		glGetBufferParameteriv(m_type, GL_BUFFER_SIZE, &sz);
		return sz;
	}
}