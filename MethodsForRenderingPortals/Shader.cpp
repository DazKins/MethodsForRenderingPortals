#include "Shader.h"

#include<glad/glad.h>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

std::string loadMainVertexShaderSource ()
{
	std::ifstream vertexShaderFile ("shaders/main.vs");
	std::string vertexShaderSource ((std::istreambuf_iterator<char> (vertexShaderFile)), (std::istreambuf_iterator<char> ()));
	return vertexShaderSource;
}

std::string loadMainFragmentShaderSource ()
{
	std::ifstream fragmentShaderFile ("shaders/main.fs");
	std::string fragmentShaderSource ((std::istreambuf_iterator<char> (fragmentShaderFile)), (std::istreambuf_iterator<char> ()));
	return fragmentShaderSource;
}

Shader::Shader ()
{

}

Shader::Shader (std::string shaderPath)
{
	std::string mainVertexShaderSource = loadMainVertexShaderSource ();
	std::string mainFragmentShaderSource = loadMainFragmentShaderSource ();

	std::ifstream vertexShaderFile (shaderPath + ".vs");
	std::string vertexShaderSource ((std::istreambuf_iterator<char> (vertexShaderFile)), (std::istreambuf_iterator<char> ()));

	std::ifstream fragmentShaderFile (shaderPath + ".fs");
	std::string fragmentShaderSource ((std::istreambuf_iterator<char> (fragmentShaderFile)), (std::istreambuf_iterator<char> ()));

	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
	const char* vertexShaderInput[2] = { mainVertexShaderSource.c_str (), vertexShaderSource.c_str() };
	int vertexShaderLengths[2] = { mainVertexShaderSource.length (), vertexShaderSource.length () };
	glShaderSource (vertexShaderId, 2, vertexShaderInput, vertexShaderLengths);
	glCompileShader (vertexShaderId);

	char infoLog[512];
	int success = 0;
	glGetShaderiv (vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog (vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED, " << shaderPath << std::endl << infoLog << std::endl;
	};

	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);
	const char* fragmentShaderInput[2] = { mainFragmentShaderSource.c_str(), fragmentShaderSource.c_str() };
	int fragmentShaderLengths[2] = { mainFragmentShaderSource.length (), fragmentShaderSource.length () };
	glShaderSource (fragmentShaderId, 2, fragmentShaderInput, fragmentShaderLengths);
	glCompileShader (fragmentShaderId);

	glGetShaderiv (fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog (fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED, " << shaderPath << std::endl << infoLog << std::endl;
	};

	this->shaderProgramId = glCreateProgram ();

	glAttachShader (shaderProgramId, vertexShaderId);
	glAttachShader (shaderProgramId, fragmentShaderId);
	glLinkProgram (shaderProgramId);

	glDeleteShader (vertexShaderId);
	glDeleteShader (fragmentShaderId);
}

std::vector<Shader> Shader::ALL_SHADERS;

Shader Shader::DEFAULT;
Shader Shader::PORTAL_CLIP;
Shader Shader::PORTAL_STENCIL_BUFFER;
Shader Shader::PORTAL_FRAMEBUFFER_OBJECT;

void Shader::initShaders ()
{
	Shader::DEFAULT = Shader ("shaders/default");
	ALL_SHADERS.push_back (Shader::DEFAULT);

	Shader::PORTAL_CLIP = Shader ("shaders/portal_clip");
	ALL_SHADERS.push_back (Shader::PORTAL_CLIP);

	Shader::PORTAL_STENCIL_BUFFER = Shader ("shaders/portal_stencil_buffer");
	ALL_SHADERS.push_back (Shader::PORTAL_STENCIL_BUFFER);

	Shader::PORTAL_FRAMEBUFFER_OBJECT = Shader ("shaders/portal_framebuffer_object");
	ALL_SHADERS.push_back (Shader::PORTAL_FRAMEBUFFER_OBJECT);

	glm::mat4 id = glm::mat4 (1.0f);

	for (Shader s : ALL_SHADERS)
	{
		s.bind ();
		Shader::setUniform ("projectionMatrix", id);
		Shader::setUniform ("viewMatrix", id);
		Shader::setUniform ("modelMatrix", id);
	}
}

void Shader::setUniform (const char* uniform, glm::mat4 matrix)
{
	int uniformLocation = glGetUniformLocation (Shader::currentlyBound, uniform);
	glUniformMatrix4fv (uniformLocation, 1, GL_FALSE, glm::value_ptr (matrix));
}

void Shader::setUniform (const char* uniform, glm::vec3 vector)
{
	int uniformLocation = glGetUniformLocation (Shader::currentlyBound, uniform);
	glUniform3fv (uniformLocation, 1, glm::value_ptr (vector));
}

void Shader::setUniform (const char* uniform, glm::vec4 vector)
{
	int uniformLocation = glGetUniformLocation (Shader::currentlyBound, uniform);
	glUniform4fv (uniformLocation, 1, glm::value_ptr (vector));
}

void Shader::setUniform (const char* uniform, bool boolean)
{
	int uniformLocation = glGetUniformLocation (Shader::currentlyBound, uniform);
	glUniform1i (uniformLocation, boolean);
}

unsigned int Shader::currentlyBound = -1;

void Shader::bind ()
{
	if (currentlyBound != this->shaderProgramId)
	{
		glUseProgram (this->shaderProgramId);
		Shader::currentlyBound = this->shaderProgramId;
	}
}

Shader::~Shader ()
{
}
