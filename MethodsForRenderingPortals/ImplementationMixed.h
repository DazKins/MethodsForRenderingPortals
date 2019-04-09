#pragma once

#include "ImplementationMixed.h"
#include "Implementation.h"

class ImplementationMixed : public Implementation
{
public:
	ImplementationMixed (Input* input, Window* window, int textureSize, int maxRecursionDepth, int cutoff, bool manualCamera);
	~ImplementationMixed ();

	void render ();
	void tick ();

private:
	unsigned int cutoff;
	unsigned int textureSize;

	static void renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, unsigned int* inPortalTextures,
		unsigned int* inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff, Window* window, glm::mat4* viewOperators);

	unsigned int* portalFrameBuffers;
	unsigned int* portalTextures;
};