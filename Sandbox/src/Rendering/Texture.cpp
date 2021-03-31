#include "Texture.hpp"

#include "stb_image/stb_image.h"

#include "Core.hpp"

Texture::Texture(int width, int height) : m_RendererID(0), xRes(width), yRes(height), m_BPP(4) {

	m_LocalBuffer = (Color*) malloc(xRes * yRes * m_BPP);

	for (int i = 0; i < xRes * yRes; i++) {
		m_LocalBuffer[i].set(255, 255, 255, 255);
	}

	// create + bind texture object
	GL_CALL(glGenTextures(1, &m_RendererID));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// tell OpenGL how to deal with textures
	// linear interpolation from texels to pixels when shrinking / growing
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
							GL_CLAMP_TO_EDGE)); // stretch x coord to edge
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
							GL_CLAMP_TO_EDGE)); // stretch y coord to edge

	// writes a 2D image to the currently bound texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xRes, yRes, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						 m_LocalBuffer));

	// unbind texture
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

// Creates a texture from the image at the given filepath.
Texture::Texture(const std::string& path)
	: m_RendererID(0), m_LocalBuffer(nullptr), xRes(0), yRes(0), m_BPP(0) {

	// read image into memory
	stbi_set_flip_vertically_on_load(1); // OpenGL bottom-left is 0, 0
	m_LocalBuffer = (Color*) stbi_load(path.c_str(), &xRes, &yRes, &m_BPP, 4);

	// create + bind texture object
	GL_CALL(glGenTextures(1, &m_RendererID));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// tell OpenGL how to deal with textures
	// linear interpolation from texels to pixels when shrinking / growing
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
							GL_CLAMP_TO_EDGE)); // stretch x coord to edge
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
							GL_CLAMP_TO_EDGE)); // stretch y coord to edge

	// writes a 2D image to the currently bound texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xRes, yRes, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						 m_LocalBuffer));

	// unbind texture
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
	GL_CALL(glDeleteTextures(1, &m_RendererID));

	delete (m_LocalBuffer);
}

// Binds a texture into a given texture slot. Default is 0.
void Texture::Bind(unsigned int slot /* = 0*/) const {
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::write(int texelX, int texelY, Color color) {
	// TODO: This should be aware of blending with whatever color is
	// already there (maybe?)
	if (texelX < 0 || texelX > xRes || texelY < 0 || texelY > yRes) {
		ARTSY_ASSERT(false, "Attempting to draw outside draw area");
		return;
	}

	m_LocalBuffer[texelX + xRes * texelY].set(color);
	// Bind();
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xRes, yRes, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						 m_LocalBuffer));
}

Color& Texture::get(int texelX, int texelY) {
	return m_LocalBuffer[texelX + xRes * texelY];
}
