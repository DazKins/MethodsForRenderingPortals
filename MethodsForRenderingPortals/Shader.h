#pragma once

#include <glm/glm.hpp>

class Shader
{
private:
	unsigned int shaderProgramId;

public:
	Shader (const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader ();

	void setUniform (const char* uniform, glm::mat4 matrix);

	void bind ();

	static Shader* DEFAULT_SHADER;
	static void initShaders ();
};