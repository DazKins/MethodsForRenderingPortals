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

private:
	void renderFromPerspective (glm::mat4 viewMatrix);
	void renderPortalView (glm::mat4 viewMatrix, Portal *inPortal, Portal *outPortal);
};

