#pragma once

#include "Application/Layer.hpp"

#include "Buffers/VertexBuffer.hpp"
#include "Buffers/VertexArray.hpp"
#include "Buffers/IndexBuffer.hpp"

#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"

#include "Events/Event.hpp"
#include "Events/MouseEvent.hpp"

#include "glm/vec2.hpp"

class DrawLayer : public Layer {
  private:
	static constexpr float LEFT_DRAW_BOUND = 100.0;
	static constexpr float LOWER_DRAW_BOUND = 100.0;
	int m_Width;
	int m_Height;

	void* m_Data;

	// TODO: mouseButtonPressedEvents should be able to get mouse coords
	int mouseX;
	int mouseY;

	Texture texture;
	Color drawColor;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexBufferLayout* layout;
	VertexArray* vertexArray;

	Renderer renderer;
	Shader shader;

	glm::vec2 pixelToTexel(float x, float y);

	bool OnMouseClick(MouseButtonPressedEvent& e);
	bool OnMouseMove(MouseMovedEvent& e);

  public:
	DrawLayer(int width, int height);
	~DrawLayer();

	void OnEvent(Event& e) override;
	void OnUpdate() override;
};
