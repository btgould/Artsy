#pragma once

#include "glutil.hpp"
#include "Drawing/Color.hpp"

// Class representing a texture. Textures are data that can be applied to a
// surface and sampled by a fragment shader to determing the color of some
// pixel.
class Texture {
  private:
	unsigned int m_RendererID;
	Color* m_LocalBuffer;
	int xRes, yRes, m_BPP;

  public:
	Texture(int width, int height);
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetXRes() const {
		return xRes;
	}

	inline int GetYRes() const {
		return yRes;
	}

	void write(int texelX, int texelY, Color color);
};
