#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

enum class ShaderType {
	NONE = -1,
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
};

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::string m_ShaderSource;
	std::unordered_map<std::string,int> m_UniformLoactionCache;
public:
	Shader() : m_RendererID(0) {};
	~Shader();

	void Bind() const;
	void UnBind() const;
	void CreateShader(const std::string& filepath);

	//Set uniforms
	void SetUniform1i(const char* name, int value);
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);

	unsigned int GetRendererID() const { return m_RendererID; }
private:
	int GetUniformLocation(const char* name);

	static unsigned int CompileShader(unsigned int type, std::string& source);
	static ShaderProgramSource ParseShader(const std::string& filepath);
public:
	void SetUniformMat4f(const char* name, glm::mat4 proj);
};