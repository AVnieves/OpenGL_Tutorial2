#pragma once
#include <string>
#include <unordered_map>
#include "GL/glew.h"
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	int GetUniformLocation(const std::string& name);
public:
	Shader(const std::string& filepath);
	~Shader();
	unsigned int CreateShader(const std::string& vs, const std::string& fs);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);

	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};
