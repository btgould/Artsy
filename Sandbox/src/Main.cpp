#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/Application.hpp"

#include "Windows/ImGuiLayer.hpp"
#include "Drawing/DrawLayer.hpp"

int main(void) {

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "[ERROR]: GLFW failed to initialize" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Application app;

	if (glewInit() != GLEW_OK)
		std::cout << "[ERROR]: GLEW failed to initialize" << std::endl;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	class LoggingLayer : public Layer {
		void OnEvent(Event& e) {
			std::cout << e << std::endl;
		}
	};

	// app.PushLayer(new LoggingLayer());
	app.PushLayer(new DrawLayer(400, 400));

	app.PushOverlay(new ImGuiLayer());

	app.run();

	return 0;
}
