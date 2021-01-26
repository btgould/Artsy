#include "IndexBuffer.hpp"

#include "glutil.hpp"

// Class to encapsulate the representation of an index buffer. Index buffers
// keep track of the indexes of which vertices to render, and the order to do
// so.
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count) {
	GL_CALL(glGenBuffers(1, &m_RendererID));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data,
						 GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GL_CALL(glDeleteBuffers(1, &m_RendererID))
}

void IndexBuffer::Bind() const {
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
