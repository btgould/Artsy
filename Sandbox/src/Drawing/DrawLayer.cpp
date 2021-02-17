#include "Drawing/DrawLayer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Core.hpp"
// #include "Application/Application.hpp"

#include "iostream"

DrawLayer::DrawLayer(int width, int height)
	: m_Width(width), m_Height(height),
	  shader("Sandbox/res/shaders/Basic.shader"),
	  texture(20, 20), /*,
	  texture("Sandbox/res/textures/Code.png"), */
	  drawColor(255, 0, 0, 255) {
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

void DrawLayer::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(DrawLayer::OnMouseMove));
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		BIND_EVENT_FN(DrawLayer::OnMouseClick));
}

void DrawLayer::OnUpdate() {
	renderer.Clear();
	renderer.Draw(*vertexArray, *indexBuffer, shader);
}

glm::vec2 DrawLayer::pixelToTexel(float x, float y) {
	int texelX = (x - LEFT_DRAW_BOUND) / texture.GetXRes();
	int texelY = (LOWER_DRAW_BOUND + m_Height - y) / texture.GetYRes();

	// check if outside draw area
	// TODO: I need a better way to signal this
	if (texelX < 0 || texelX >= texture.GetXRes() || texelY < 0 ||
		texelY >= texture.GetYRes()) {
		texelX = -1;
		texelY = -1;
	}

	return glm::vec2(texelX, texelY);
}

bool DrawLayer::OnMouseClick(MouseButtonPressedEvent& e) {
	glm::vec2 texelCoords = pixelToTexel(mouseX, mouseY);
	std::cout << "Texel Clicked: " << texelCoords.x << ", " << texelCoords.y
			  << std::endl;
	texture.write(texelCoords.x, texelCoords.y, drawColor);
	return false;
}

bool DrawLayer::OnMouseMove(MouseMovedEvent& e) {
	mouseX = e.GetX();
	mouseY = e.GetY();
	return false;
}
