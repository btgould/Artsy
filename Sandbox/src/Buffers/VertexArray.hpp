#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

// Class that represents a vertex array. Vertex arrays associate vertex buffers
// with layouts of vertex attributes that describe the type and location of
// information in each vertex. 
class VertexArray {
  private:
	unsigned int m_RendererID;

  public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
