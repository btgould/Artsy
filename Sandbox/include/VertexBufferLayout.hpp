#pragma once

#include <vector>

#include "glutil.hpp"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getTypeSize(unsigned int type) {
		switch (type) {
		case GL_FLOAT:
			return 4;
			break;
		case GL_UNSIGNED_INT:
			return 4;
			break;
		case GL_UNSIGNED_BYTE:
			return 1;
			break;

		default:
			ASSERT(false);
			return 0;
			break;
		}
	}
};

class VertexBufferLayout {
  private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

  public:
	VertexBufferLayout() : m_Stride(0) {};
	~VertexBufferLayout() {};

	template <typename T> void Push(unsigned int count) {
		// do not allow execution w/o defining a specific type
		static_assert(sizeof(T) != 0,
					  "This function must be defined for a specific type");
	}

	inline const std::vector<VertexBufferElement>& getElements() const {
		return m_Elements;
	}

	inline unsigned int getStride() const {
		return m_Stride;
	}
};

template <> inline void VertexBufferLayout::Push<float>(unsigned int count) {
	m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
	m_Stride += VertexBufferElement::getTypeSize(GL_FLOAT) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
	m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
	m_Stride += VertexBufferElement::getTypeSize(GL_UNSIGNED_INT) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
	m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
	m_Stride += VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE) * count;
}
