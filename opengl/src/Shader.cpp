#include "Shader.h"
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Debug.h"


Shader::~Shader()
{
	
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const char* name, int value)
{
	unsigned int location = GetUniformLocation(name);
	GLCall(glUniform1i(location, value));
}

void Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	unsigned int location = GetUniformLocation(name);
	GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const char* name, glm::mat4 proj)
{
	unsigned int location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location,1,GL_FALSE,&proj[0][0]));
}

int Shader::GetUniformLocation(const char* name)
{
	if (m_UniformLoactionCache.find(name) != m_UniformLoactionCache.end()) 
		return m_UniformLoactionCache[name];

	int location = glGetUniformLocation(m_RendererID, (const GLchar*)name);
	if (location == -1){
		LOG("Warning: uniform ");
		LOG(name);
		LOG("doesn't exist!");
	}

	return location;
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath){
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(),ss[1].str() };
}





//±àÒëShader 
unsigned int Shader::CompileShader(unsigned int type,std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::CreateShader(const std::string& filepath) {
	ShaderProgramSource source = Shader::ParseShader(filepath);
	
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	//std::cout << program << std::endl;
	m_RendererID = program;
}