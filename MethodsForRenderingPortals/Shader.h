#pragma once
class Shader
{
private:
	const char* vertexShaderSource;
	const char* fragmentShaderSource;

	unsigned int shaderProgramId;

public:
	Shader (const char* vertexShader, const char* fragmentShader);
	~Shader ();

	void bind ();
};

