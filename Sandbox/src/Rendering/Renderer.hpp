#pragma once

#include "Buffers/VertexArray.hpp"
#include "Buffers/IndexBuffer.hpp"
#include "Shader.hpp"

// Class representing a renderer to simplify OpenGL draw calls.
class Renderer {
  public:
	Renderer();
	~Renderer();

	void Clear() const;

	void Draw(const VertexArray& va, const IndexBuffer& ib,
			  const Shader& shader) const;
};
