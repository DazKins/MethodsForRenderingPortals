#pragma once
class Shader
{
private:
	unsigned int shaderProgramId;

public:
	Shader (const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader ();

	void bind ();
};

