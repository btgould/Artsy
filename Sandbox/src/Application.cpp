#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "glutil.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "[ERROR]: GLFW failed to initialize" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window) {
		GL_CALL(glfwTerminate());
		std::cout << "[ERROR]: Window could not be created" << std::endl;
		return -1;
	}

	/* Make the window's context current */
	GL_CALL(glfwMakeContextCurrent(window));

	// don't update buffers more than once per screen update
	glfwSwapInterval(1);

	// enable blending transparent values
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	if (glewInit() != GLEW_OK)
		std::cout << "[ERROR]: GLEW failed to initialize" << std::endl;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// positions of vertices to render
	float positions[] = {
		100.0f, 100.0f, 0.0f, 0.0f, // 0
		200.0f, 100.0f, 1.0f, 0.0f, // 1
		200.0f, 200.0f, 1.0f, 1.0f, // 2
		100.0f, 200.0f, 0.0f, 1.0f	// 3
	};

	// indices of vertices to render
	unsigned int indices[] {0, 1, 2, 2, 3, 0};

	// create vertex buffer from positions array
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	// create and define the layout of our vertexes (2 floats per vertex)
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.addBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	// TRANSFORM: represents tanslation, rotation, and scale of something
	// model matrix describes transform of object being drawn
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	// view matrix describes transform of camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	// projection matrix maps vertex positions to rendering space([-1, 1])
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

	glm::mat4 mvp = proj * view * model;

	Shader shader("Sandbox/res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader.SetUniformMat4f("u_MVP", mvp);

	Texture texture("Sandbox/res/textures/Code.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	vb.Unbind();
	ib.Unbind();
	va.Unbind();
	shader.Unbind();

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		renderer.Clear();

		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		// update color
		if (r > 1)
			increment = -0.05f;
		else if (r < 0)
			increment = 0.05f;

		r += increment;

		renderer.Draw(va, ib, shader);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
