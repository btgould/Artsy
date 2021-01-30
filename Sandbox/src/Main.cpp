#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "glutil.hpp"
#include "Buffers/VertexBuffer.hpp"
#include "Buffers/IndexBuffer.hpp"
#include "Buffers/VertexArray.hpp"

#include "Rendering/Shader.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Texture.hpp"

#include "Application/Application.hpp"

int main(void) {

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "[ERROR]: GLFW failed to initialize" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates an OpenGL window and initializes the context
	Application app;

	class LoggingLayer : public Layer {
		void OnEvent(Event& e) {
			std::cout << e << std::endl;
		}
	};

	app.PushLayer(new LoggingLayer());

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
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	// projection matrix maps vertex positions to rendering space([-1, 1])
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

	glm::mat4 mvp = proj * view * model;

	Shader shader("Sandbox/res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("Sandbox/res/textures/Code.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	vb.Unbind();
	ib.Unbind();
	va.Unbind();
	shader.Unbind();

	Renderer renderer;

	glm::vec3 translation(200, 200, 0);

	class RendererLayer : public Layer {
		void OnUpdate() {
			ImGui_ImplGlfwGL3_NewFrame();

			{
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
							1000.0f / ImGui::GetIO().Framerate,
							ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
	};

	app.PushLayer(new RendererLayer());

	app.run();

	/* Loop until the user closes the window */
	// while (!glfwWindowShouldClose(window)) {
	// 	/* Render here */
	// 	renderer.Clear();

	// 	ImGui_ImplGlfwGL3_NewFrame();

	// 	model = glm::translate(glm::mat4(1.0f), translation);
	// 	mvp = proj * view * model;

	// 	shader.Bind();
	// 	shader.SetUniformMat4f("u_MVP", mvp);

	// 	renderer.Draw(va, ib, shader);

	// 	// 1. Show a simple window.
	// 	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets
	// 	// automatically appears in a window called "Debug".
	// 	{
	// 		ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);

	// 		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
	// 					1000.0f / ImGui::GetIO().Framerate,
	// 					ImGui::GetIO().Framerate);
	// 	}

	// 	ImGui::Render();
	// 	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	// 	/* Swap front and back buffers */
	// 	glfwSwapBuffers(window);

	// 	/* Poll for and process events */
	// 	glfwPollEvents();
	// }

	return 0;
}
