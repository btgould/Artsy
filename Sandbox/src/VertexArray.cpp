#include "VertexArray.hpp"

#include "glutil.hpp"

VertexArray::VertexArray() {
	GL_CALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}

// Associates the given layout with the given vertex buffer. Also binds the
// vertex buffer as a side effect.
void VertexArray::addBuffer(const VertexBuffer& vb,
							const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();

	const auto& elements = layout.getElements();

	unsigned int offset = 0;

	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		// enable + bind each vertex attribute to the current buffer
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(
			i, element.count, element.type, element.normalized,
			layout.getStride(),
			(const void*) offset)); // associates attribute w/ buffer

		offset +=
			element.count * VertexBufferElement::getTypeSize(element.type);
	}
}

void VertexArray::Bind() const {
	GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GL_CALL(glBindVertexArray(0));
}
