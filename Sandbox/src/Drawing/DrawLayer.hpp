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
#include "Events/KeyEvent.hpp"

#include "Drawing/UndoRedo.hpp"

#include "glm/vec2.hpp"
#include "imgui/imgui.h"

class DrawLayer : public Layer {
  private:
	static constexpr float LEFT_DRAW_BOUND = 100.0;
	static constexpr float LOWER_DRAW_BOUND = 100.0;
	int m_Width;
	int m_Height;

	// TODO: mouseButtonPressedEvents should be able to get mouse coords
	int mouseX;
	int mouseY;

	Texture texture;
	Color drawColor;
	ImVec4 drawColorGui = {0, 0, 0, 1};

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexBufferLayout* layout;
	VertexArray* vertexArray;

	Renderer renderer;
	Shader shader;

	bool m_Drawing;

	Operation localOpStack[50 * 50 + 1];
	unsigned int currOpSize = 0;
	OperationStack undoStack;

	TexelCoords pixelToTexel(float x, float y);

	bool OnMouseClick(MouseButtonPressedEvent& e);
	bool OnMouseRelease(MouseButtonReleasedEvent& e);
	bool OnMouseMove(MouseMovedEvent& e);

	bool OnKeyPress(KeyPressedEvent& e);

  public:
	DrawLayer(int width, int height);
	~DrawLayer();

	void undoOperation(const Operation* const ops);
	void redoOperation(const Operation* const ops);

	void OnEvent(Event& e) override;
	void OnUpdate() override;
	void OnImGuiRender() override;
};
