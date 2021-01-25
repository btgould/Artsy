#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "glutil.hpp"

Shader::Shader(const std::string& filepath) : m_Filepath(filepath) {
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
	GL_CALL(glDeleteProgram(m_RendererID));
}

// compiles a shader from source code and returns its ID
unsigned int Shader::CompileShader(unsigned int type,
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
unsigned int Shader::CreateShader(const std::string& vertexShader,
								  const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDeleteShader(vs));
	GL_CALL(glDeleteShader(fs));

	return program;
}

// get source for each shader from shader file
ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
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

void Shader::Bind() const {
	GL_CALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GL_CALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2,
						  float v3) {
	GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GL_CALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1)
		std::cout << "[WARNING]: Uniform " << name << " not found" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
