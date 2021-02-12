#pragma once

#include "Application/Layer.hpp"

#include "Buffers/VertexBuffer.hpp"
#include "Buffers/VertexArray.hpp"
#include "Buffers/IndexBuffer.hpp"

#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"

class DrawLayer : public Layer {
  private:
	static constexpr float LEFT_DRAW_BOUND = 100.0;
	static constexpr float LOWER_DRAW_BOUND = 100.0;
	int m_Width;
	int m_Height;

	void* m_Data;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexBufferLayout* layout;
	VertexArray* vertexArray;

	Texture texture;

	Renderer renderer;
	Shader shader;

  public:
	DrawLayer(int width, int height);
	~DrawLayer();

	void OnUpdate() override;
};
