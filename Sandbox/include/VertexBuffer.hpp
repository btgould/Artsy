#pragma once

// Class to encapsulate the representation of a vertex buffer. Vertex buffers
// store data about "vertices", which are representations of sections of the
// model that is to be rendered. They store information such as position,
// material, normals, etc. as raw data according to the layout specified by the
// current vertex array buffer.
class VertexBuffer {
  private:
	unsigned int m_RendererID;

  public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
