#include "Drawing/DrawLayer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "iostream"

DrawLayer::DrawLayer(int width, int height)
	: m_Width(width), m_Height(height),
	  shader("Sandbox/res/shaders/Basic.shader"),
	  texture("Sandbox/res/textures/Code.png") {

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

	shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
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

void DrawLayer::OnUpdate() {
	renderer.Clear();
	renderer.Draw(*vertexArray, *indexBuffer, shader);
}
