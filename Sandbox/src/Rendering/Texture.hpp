#pragma once

#include "glutil.hpp"

#include "glm/vec2.hpp"

#include "Drawing/Color.hpp"

struct TexelCoords {
	bool onTex;
	glm::vec2 coords;

	TexelCoords(bool onTexture, glm::vec2 coordinates) : onTex(onTexture), coords(coordinates) {}
};

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

	void Set(std::string path);

	inline Color* getData() const {
		return m_LocalBuffer;
	}

	inline int GetXRes() const {
		return xRes;
	}

	inline int GetYRes() const {
		return yRes;
	}

	inline int getComp() const {
		return m_BPP;
	}

	void write(int texelX, int texelY, Color color);
	Color& get(int texelX, int texelY);
};
