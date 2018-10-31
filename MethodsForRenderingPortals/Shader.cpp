#include "Shader.h"

#include<glad/glad.h>

Shader::Shader (const char* vertexShader, const char* fragmentShader)
{
	this->vertexShaderSource = vertexShader;
	this->fragmentShaderSource = fragmentShader;

	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShaderId, 1, &this->vertexShaderSource, NULL);
	glCompileShader (vertexShaderId);

	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShaderId, 1, &this->fragmentShaderSource, NULL);
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
