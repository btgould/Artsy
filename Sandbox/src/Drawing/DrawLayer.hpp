#pragma once

#include "Application/Layer.hpp"

#include "Buffers/VertexBuffer.hpp"
#include "Buffers/VertexArray.hpp"
#include "Buffers/IndexBuffer.hpp"

#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"

class DrawLayer : public Layer {
  private:
	static const int LEFT_DRAW_BOUND;
	static const int UPPER_DRAW_BOUND;
	int m_Width;
	int m_Height;

	void* m_Data;

	VertexBuffer* drawArea;
	VertexArray* drawLayout;
	IndexBuffer* indexBuffer;

	Renderer renderer;
	Shader shader;

  public:
	DrawLayer(int width, int height, std::string shaderPath);
	~DrawLayer();

	void OnUpdate() override;
};
