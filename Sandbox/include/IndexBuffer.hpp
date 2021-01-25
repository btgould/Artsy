#pragma once

// Class to encapsulate the representation of an index buffer. Index buffers
// keep track of the indexes of which vertices to render, and the order to do
// so.
class IndexBuffer {
  private:
	unsigned int m_RendererID;
	unsigned int m_Count;

  public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const {
		return m_Count;
	}
};
