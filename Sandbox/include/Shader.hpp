#pragma once

#include "string"
#include <unordered_map>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

// Class that represents a shader. Shaders run on the GPU and tell the computer
// how to render each pixel. Currently, two types of shaders are supported:
// vertex and fragment. Vertex shaders are run once for each vertex we render,
// and fragment shaders are run for each pixel in the enclosed area.
class Shader {
  private:
	std::string m_Filepath; // debug only
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

  public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Setting uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2,
					  float v3);

  private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader,
							  const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};
