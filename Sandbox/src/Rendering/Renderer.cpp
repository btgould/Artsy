#include "Renderer.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Clear() const {
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib,
					const Shader& shader) const {
	va.Bind();
	ib.Bind();
	shader.Bind();

	GL_CALL(
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
