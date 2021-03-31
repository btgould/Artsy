#include "Drawing/DrawLayer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TEMP: needed for key mods in key pressed callback
#include <GLFW/glfw3.h>

#include "Core.hpp"
// #include "Application/Application.hpp"

#include "iostream"

DrawLayer::DrawLayer(int width, int height)
	: m_Width(width), m_Height(height), m_Drawing(false),
	  shader("Sandbox/res/shaders/Basic.shader"), texture(50, 50)
/*texture("Sandbox/res/textures/Code.png")*/ {
	// This HAS to be a float array, or OpenGL misinterprets it
	float drawCorners[] = {
		LEFT_DRAW_BOUND,		 LOWER_DRAW_BOUND,			0, 0, // 0
		LEFT_DRAW_BOUND + width, LOWER_DRAW_BOUND,			1, 0, // 1
		LEFT_DRAW_BOUND + width, LOWER_DRAW_BOUND + height, 1, 1, // 2
		LEFT_DRAW_BOUND,		 LOWER_DRAW_BOUND + height, 0, 1  // 3
	};

	unsigned int drawOrder[] = {0, 1, 2, 2, 3, 0};
	indexBuffer = new IndexBuffer(drawOrder, 6);

	vertexBuffer = new VertexBuffer(drawCorners, sizeof(drawCorners));

	layout = new VertexBufferLayout();
	layout->Push<float>(2); // vertex pos coords
	layout->Push<float>(2); // tex coords

	vertexArray = new VertexArray();
	vertexArray->addBuffer(*vertexBuffer, *layout);

	// TRANSFORM: represents tanslation, rotation, and scale of something
	// model matrix describes transform of object being drawn
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	// view matrix describes transform of camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	// projection matrix maps vertex positions to rendering space([-1, 1])
	glm::mat4 proj = glm::ortho(0.0f, 940.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	glm::mat4 mvp = proj * view * model;

	shader.Bind();

	shader.SetUniformMat4f("u_MVP", mvp);

	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);
}

DrawLayer::~DrawLayer() {
	delete vertexBuffer;
	delete indexBuffer;
	delete layout;
	delete vertexArray;
}

void DrawLayer::undoOperation(const Operation* const ops) {
	if (ops) {
		int i = 0;

		while (ops[i].getType() != OPTYPE_MOUSE_UP) {
			Color newColor = texture.get(ops[i].getPxChanged().x, ops[i].getPxChanged().y) -
							 ops[i].getColorDiff();

			texture.write(ops[i].getPxChanged().x, ops[i].getPxChanged().y, newColor);
			i++;
		}
	}
}

void DrawLayer::redoOperation(const Operation* const ops) {
	if (ops) {
		int i = 0;

		while (ops[i].getType() != OPTYPE_MOUSE_UP) {
			Color newColor = texture.get(ops[i].getPxChanged().x, ops[i].getPxChanged().y) +
							 ops[i].getColorDiff();

			texture.write(ops[i].getPxChanged().x, ops[i].getPxChanged().y, newColor);
			i++;
		}
	}
}

void DrawLayer::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(DrawLayer::OnMouseMove));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(DrawLayer::OnMouseClick));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(DrawLayer::OnMouseRelease));

	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(DrawLayer::OnKeyPress));
}

void DrawLayer::OnUpdate() {
	renderer.Clear();
	renderer.Draw(*vertexArray, *indexBuffer, shader);
}

void DrawLayer::OnImGuiRender() {
	// BUG: for some reason, transparency does not work in the color preview
	ImGui::ColorEdit4("Draw Color", &drawColorGui.x);
	drawColor.set((unsigned char) (drawColorGui.x * 255), (unsigned char) (drawColorGui.y * 255),
				  (unsigned char) (drawColorGui.z * 255), (unsigned char) (drawColorGui.w * 255));

	if (ImGui::Button("Undo")) {
		undoOperation(undoStack.popUndo());
	}

	ImGui::SameLine();

	if (ImGui::Button("Redo")) {
		redoOperation(undoStack.popRedo());
	}
}

TexelCoords DrawLayer::pixelToTexel(float x, float y) {
	int texelX = (x - LEFT_DRAW_BOUND) * texture.GetXRes() / m_Width;
	int texelY = (LOWER_DRAW_BOUND + m_Height - y) * texture.GetYRes() / m_Height;

	TexelCoords txCoords(true, glm::vec2(texelX, texelY));

	// check if outside draw area
	if (texelX < 0 || texelX >= texture.GetXRes() || texelY < 0 || texelY >= texture.GetYRes()) {
		txCoords.onTex = false;
	}

	return txCoords;
}

bool DrawLayer::OnMouseClick(MouseButtonPressedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();

	// checks if over ImGui window
	if (io.WantCaptureMouse) {
		return true;
	}

	if (e.GetMouseButton() == 0) {
		TexelCoords txCoords = pixelToTexel(mouseX, mouseY);
		// std::cout << "Texel Clicked: " << texelCoords.x << ", " << texelCoords.y << std::endl;

		if (txCoords.onTex) {
			glm::vec2 pxChanged = txCoords.coords;

			// check if operation actually changes anything
			Color oldColor = texture.get(pxChanged.x, pxChanged.y);

			if (oldColor != drawColor) {
				// save operation to local buffer
				localOpStack[currOpSize] =
					Operation(OPTYPE_MOUSE_DOWN, pxChanged, drawColor - oldColor);
				currOpSize++;

				// write draw color to texture
				texture.write(txCoords.coords.x, txCoords.coords.y, drawColor);
			}

			m_Drawing = true;
		}
	}

	return false;
}

bool DrawLayer::OnMouseRelease(MouseButtonReleasedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();

	// checks if over ImGui window
	if (io.WantCaptureMouse) {
		return true;
	}

	if (e.GetMouseButton() == 0 && m_Drawing) {
		m_Drawing = false;

		// cap + push operation if needed
		if (currOpSize != 0) {
			localOpStack[currOpSize] = Operation(OPTYPE_MOUSE_UP, glm::vec2(0, 0), Color());
			undoStack.pushUndo(localOpStack);
			currOpSize = 0;
		}
	}

	return false;
}

bool DrawLayer::OnMouseMove(MouseMovedEvent& e) {
	mouseX = e.GetX();
	mouseY = e.GetY();

	if (m_Drawing) {
		TexelCoords txCoords = pixelToTexel(mouseX, mouseY);

		if (txCoords.onTex) {
			glm::vec2 pxChanged = txCoords.coords;

			// check if operation actually changes anything
			Color oldColor = texture.get(pxChanged.x, pxChanged.y);

			if (oldColor != drawColor) {
				// save operation to local buffer
				localOpStack[currOpSize] = Operation(OPTYPE_DRAW, pxChanged, drawColor - oldColor);
				currOpSize++;

				// write draw color to texture
				texture.write(txCoords.coords.x, txCoords.coords.y, drawColor);
			}
		}
	}

	return false;
}

bool DrawLayer::OnKeyPress(KeyPressedEvent& e) {
	int key = e.GetKeyCode();

	// TODO: find a better way of binding actions to keys
	if (key == GLFW_KEY_Z) {
		if (e.modDown(GLFW_MOD_CONTROL)) {
			if (e.modDown(GLFW_MOD_SHIFT)) {
				redoOperation(undoStack.popRedo());
			} else {
				undoOperation(undoStack.popUndo());
			}
		}
	}

	return false;
}
