#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader
{
private:
	unsigned int shaderProgramId;

	static glm::mat4 currentModelMatrix;
	static glm::mat4 currentViewMatrix;
	static glm::mat4 currentProjectionMatrix;

public:
	Shader ();
	Shader (std::string shaderPath);
	~Shader ();

	static void setUniform (const char* uniform, glm::mat4 matrix);
	static void setUniform (const char* uniform, glm::vec3 vector);
	static void setUniform (const char* uniform, glm::vec4 vector);
	static void setUniform (const char* uniform, bool boolean);

	void bind ();

	static std::vector<Shader> ALL_SHADERS;

	static Shader DEFAULT;
	static Shader PORTAL_CLIP;
	static Shader PORTAL_STENCIL_BUFFER;
	static Shader PORTAL_FRAMEBUFFER_OBJECT;
	static void initShaders ();

	static glm::mat4 getCurrentModelMatrix ();
	static glm::mat4 getCurrentViewMatrix ();
	static glm::mat4 getCurrentProjectionMatrix ();

	static unsigned int currentlyBound;
};