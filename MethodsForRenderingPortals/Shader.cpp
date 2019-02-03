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

	glUseProgram (shaderProgramId);

	glDeleteShader (vertexShaderId);
	glDeleteShader (fragmentShaderId);
}

glm::mat4 Shader::currentViewMatrix;
glm::mat4 Shader::currentModelMatrix;
glm::mat4 Shader::currentProjectionMatrix;

std::vector<Shader*> Shader::ALL_SHADERS;

Shader* Shader::DEFAULT;
Shader* Shader::PORTAL_CLIP;
Shader* Shader::PORTAL_STENCIL_BUFFER;
Shader* Shader::PORTAL_FRAMEBUFFER_OBJECT;

void Shader::initShaders ()
{
	Shader::DEFAULT = new Shader ("shaders/default");
	ALL_SHADERS.push_back (Shader::DEFAULT);

	Shader::PORTAL_CLIP = new Shader ("shaders/portal_clip");
	ALL_SHADERS.push_back (Shader::PORTAL_CLIP);

	Shader::PORTAL_STENCIL_BUFFER = new Shader ("shaders/portal_stencil_buffer");
	ALL_SHADERS.push_back (Shader::PORTAL_STENCIL_BUFFER);

	Shader::PORTAL_FRAMEBUFFER_OBJECT = new Shader ("shaders/portal_framebuffer_object");
	ALL_SHADERS.push_back (Shader::PORTAL_FRAMEBUFFER_OBJECT);

	glm::mat4 id = glm::mat4 (1.0f);
	updateAllProjectionMatrices (id);
	updateAllViewMatrices (id);
	updateAllModelMatrices (id);
}

void Shader::updateAllProjectionMatrices (glm::mat4 matrix)
{
	for (Shader* s : Shader::ALL_SHADERS)
	{
		s->setUniform ("projectionMatrix", matrix);
	}

	Shader::currentProjectionMatrix = matrix;
}

void Shader::updateAllViewMatrices (glm::mat4 matrix)
{
	for (Shader* s : Shader::ALL_SHADERS)
	{
		s->setUniform ("viewMatrix", matrix);
	}

	Shader::currentViewMatrix = matrix;
}

void Shader::updateAllModelMatrices (glm::mat4 matrix)
{
	for (Shader* s : Shader::ALL_SHADERS)
	{
		s->setUniform ("modelMatrix", matrix);
	}

	Shader::currentModelMatrix = matrix;
}

void Shader::setUniform (const char* uniform, glm::mat4 matrix)
{
	this->bind ();

	int uniformLocation = glGetUniformLocation (this->shaderProgramId, uniform);
	glUniformMatrix4fv (uniformLocation, 1, GL_FALSE, glm::value_ptr (matrix));
}

void Shader::setUniform (const char* uniform, glm::vec3 vector)
{
	this->bind ();

	int uniformLocation = glGetUniformLocation (this->shaderProgramId, uniform);
	glUniform3fv (uniformLocation, 1, glm::value_ptr (vector));
}

void Shader::setUniform (const char* uniform, glm::vec4 vector)
{
	this->bind ();

	int uniformLocation = glGetUniformLocation (this->shaderProgramId, uniform);
	glUniform4fv (uniformLocation, 1, glm::value_ptr (vector));
}

void Shader::setUniform (const char* uniform, bool boolean)
{
	this->bind ();

	int uniformLocation = glGetUniformLocation (this->shaderProgramId, uniform);
	glUniform1i (uniformLocation, boolean);
}

glm::mat4 Shader::getCurrentModelMatrix ()
{
	return Shader::currentModelMatrix;
}

glm::mat4 Shader::getCurrentViewMatrix ()
{
	return Shader::currentViewMatrix;
}

glm::mat4 Shader::getCurrentProjectionMatrix ()
{
	return Shader::currentProjectionMatrix;
}

void Shader::bind ()
{
	glUseProgram (this->shaderProgramId);
}

Shader::~Shader ()
{
}
