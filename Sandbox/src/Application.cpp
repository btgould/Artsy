#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "glutil.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

// compiles a shader from source code and returns its ID
static unsigned int compileShader(unsigned int type,
								  const std::string& source) {
	GL_CALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GL_CALL(glShaderSource(id, 1, &src, nullptr));
	GL_CALL(glCompileShader(id));

	// check for compilation errors
	int result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* msg = (char*) alloca(length * sizeof(char));
		GL_CALL(glGetShaderInfoLog(id, length, &length, msg));
		std::cout << "Failed to compile "
				  << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment")
				  << " shader" << std::endl;
		std::cout << msg << std::endl;
		GL_CALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

// compliles shaders from source, links into a program, returns its ID
static unsigned int createShader(const std::string& vertexShader,
								 const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDeleteShader(vs));
	GL_CALL(glDeleteShader(fs));

	return program;
}

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

// get source for each shader from shader file
static ShaderProgramSource parseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	std::string line;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			// declaration of a shader -> find type
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			} else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		} else {
			// add line to current shader code
			ss[(int) type] << line << "\n";
		}
	}

	return {ss[0].str(), ss[1].str()};
}

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

	// create vertex array object
	unsigned int vao;
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));

	// creating chunk of data to store info about what to draw,
	// set it as active (bound) buffer
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	// enable first vertex attribute
	GL_CALL(glEnableVertexAttribArray(0));
	// define structure of first vertex attribute, bind to current vertex buffer
	// (index, size, type, normalized, stride (distance to next attribute),
	// pointer (offset from start of vertex))
	GL_CALL(
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	// set list of indices to draw
	IndexBuffer ib(indices, 6);

	// create shaders from source code
	ShaderProgramSource source =
		parseShader("Sandbox/res/shaders/Basic.shader");

	unsigned int shader =
		createShader(source.vertexSource, source.fragmentSource);
	GL_CALL(glUseProgram(shader));

	// get location of uniform used in shader
	GL_CALL(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		// update color
		if (r > 1)
			increment = -0.05f;
		else if (r < 0)
			increment = 0.05f;

		r += increment;

		GL_CALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		// draw currently bounds buffer using list of indices
		GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GL_CALL(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}
