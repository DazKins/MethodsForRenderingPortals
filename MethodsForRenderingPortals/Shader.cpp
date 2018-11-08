#include "Shader.h"

#include<glad/glad.h>
#include <fstream>
#include <iostream>

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

void Shader::bind ()
{
	glUseProgram (this->shaderProgramId);
}

Shader::~Shader ()
{
}
