#include "Shader.h"

#include<glad/glad.h>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader (const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::ifstream vertexShaderFile (vertexShaderPath);
	std::string vertexShaderSource ((std::istreambuf_iterator<char> (vertexShaderFile)),
		(std::istreambuf_iterator<char> ()));
	const char* vertexShaderSourceCStr = vertexShaderSource.c_str ();

	std::ifstream fragmentShaderFile (fragmentShaderPath);
	std::string fragmentShaderSource ((std::istreambuf_iterator<char> (fragmentShaderFile)),
		(std::istreambuf_iterator<char> ()));
	const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str ();

	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShaderId, 1, &vertexShaderSourceCStr, NULL);
	glCompileShader (vertexShaderId);

	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShaderId, 1, &fragmentShaderSourceCStr, NULL);
	glCompileShader (fragmentShaderId);

	this->shaderProgramId = glCreateProgram ();

	glAttachShader (shaderProgramId, vertexShaderId);
	glAttachShader (shaderProgramId, fragmentShaderId);
	glLinkProgram (shaderProgramId);

	glUseProgram (shaderProgramId);

	glDeleteShader (vertexShaderId);
	glDeleteShader (fragmentShaderId);
}

void Shader::setUniform (const char* uniform, glm::mat4 matrix)
{
	int uniformLocation = glGetUniformLocation (this->shaderProgramId, uniform);
	glUniformMatrix4fv (uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::bind ()
{
	glUseProgram (this->shaderProgramId);
}

Shader::~Shader ()
{
}
