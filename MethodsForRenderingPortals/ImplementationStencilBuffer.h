#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Implementation.h"

class ImplementationStencilBuffer : public Implementation
{
public:
	ImplementationStencilBuffer (Input* input, Window* window);
	~ImplementationStencilBuffer ();

	void render ();
	void tick ();

	void renderFromPerspective (Camera* cam);

private:
	glm::vec3 portal1Position;
	glm::vec3 portal1Normal;

	glm::vec3 portal2Position;
	glm::vec3 portal2Normal;

	VAO* portal1;
	VAO* portal2;
};

