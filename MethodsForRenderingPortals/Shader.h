#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader
{
private:
	unsigned int shaderProgramId;

	static std::vector<Shader*> ALL_SHADERS;

public:
	Shader (std::string shaderPath);
	~Shader ();

	void setUniform (const char* uniform, glm::mat4 matrix);
	void setUniform (const char* uniform, glm::vec3 vector);

	void bind ();

	static Shader* DEFAULT;
	static Shader* PORTAL_CLIP;
	static void initShaders ();

	static void updateAllProjectionMatrices (glm::mat4 matrix);
	static void updateAllViewMatrices (glm::mat4 matrix);
	static void updateAllModelMatrices (glm::mat4 matrix);
};