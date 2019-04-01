#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Implementation.h"

class ImplementationStencilBuffer : public Implementation
{
public:
	ImplementationStencilBuffer (Input* input, Window* window, int maxRecursionDepth);
	~ImplementationStencilBuffer ();

	void render ();
	void tick ();

private:
	static void renderPortalView (glm::mat4 viewMatrix, Portal *inPortal, Portal *outPortal, Level* level, int maxRecursionDepth);
};

