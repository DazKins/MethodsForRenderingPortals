#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader
{
private:
	unsigned int shaderProgramId;

	static std::vector<Shader*> ALL_SHADERS;

	static glm::mat4 currentModelMatrix;
	static glm::mat4 currentViewMatrix;
	static glm::mat4 currentProjectionMatrix;

public:
	Shader (std::string shaderPath);
	~Shader ();

	void setUniform (const char* uniform, glm::mat4 matrix);
	void setUniform (const char* uniform, glm::vec3 vector);
	void setUniform (const char* uniform, glm::vec4 vector);
	void setUniform (const char* uniform, bool boolean);

	void bind ();

	static Shader* DEFAULT;
	static Shader* PORTAL_CLIP;
	static Shader* PORTAL_STENCIL_BUFFER;
	static void initShaders ();

	static void updateAllProjectionMatrices (glm::mat4 matrix);
	static void updateAllViewMatrices (glm::mat4 matrix);
	static void updateAllModelMatrices (glm::mat4 matrix);

	static glm::mat4 getCurrentModelMatrix ();
	static glm::mat4 getCurrentViewMatrix ();
	static glm::mat4 getCurrentProjectionMatrix ();
};