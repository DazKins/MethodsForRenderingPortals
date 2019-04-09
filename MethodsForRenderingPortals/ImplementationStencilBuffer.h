#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Implementation.h"

#include <vector>

class ImplementationStencilBuffer : public Implementation
{
public:
	ImplementationStencilBuffer (Input* input, Window* window, int maxRecursionDepth, bool manualCamera);
	~ImplementationStencilBuffer ();

	void render ();
	void tick ();

private:
	static void renderPortalView (glm::mat4 viewMatrix, Portal *inPortal, Portal *outPortal, Level* level, int maxRecursionDepth, glm::mat4* viewOperators);
};

