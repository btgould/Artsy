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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		GL_CALL(glfwTerminate());
		std::cout << "[ERROR]: Window could not be created" << std::endl;
		return -1;
	}

	/* Make the window's context current */
	GL_CALL(glfwMakeContextCurrent(window));

	// don't update buffers more than once per screen update
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "[ERROR]: GLEW failed to initialize" << std::endl;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// positions of vertices to render
	float positions[8] = {
		-0.5f, -0.5f, // 0
		0.5f,  -0.5f, // 1
		0.5f,  0.5f,  // 2
		-0.5f, 0.5f	  // 3
	};

	// indices of vertices to render
	unsigned int indices[] {0, 1, 2, 2, 3, 0};

	// create vertex buffer from positions array
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	// create and define the layout of our vertexes (2 floats per vertex)
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.addBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	Shader shader("Sandbox/res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

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
