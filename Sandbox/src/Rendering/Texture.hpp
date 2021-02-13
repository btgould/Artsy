#pragma once

#include "glutil.hpp"

// Class representing a texture. Textures are data that can be applied to a
// surface and sampled by a fragment shader to determing the color of some
// pixel.
class Texture {
  private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

  public:
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const {
		return m_Width;
	}

	inline int GetHeight() const {
		return m_Height;
	}
};
