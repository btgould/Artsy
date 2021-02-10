#include "Texture.hpp"

#include "stb_image/stb_image.h"

// Creates a texture from the image at the given filepath.
Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
	  m_Height(0), m_BPP(0) {

	// read image into memory
	stbi_set_flip_vertically_on_load(1); // OpenGL bottom-left is 0, 0
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	// create + bind texture object
	GL_CALL(glGenTextures(1, &m_RendererID));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// tell OpenGL how to deal with textures
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// writes a 2D image to the currently bound texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
						 GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	// unbind texture
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
	GL_CALL(glDeleteTextures(1, &m_RendererID));
}

// Binds a texture into a given texture slot. Default is 0.
void Texture::Bind(unsigned int slot /* = 0*/) const {
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}