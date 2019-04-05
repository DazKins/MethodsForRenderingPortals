#pragma once

#include "ImplementationMixed.h"
#include "Implementation.h"

class ImplementationMixed : public Implementation
{
public:
	ImplementationMixed (Input* input, Window* window, int textureSize, int maxRecursionDepth, int cutoff);
	~ImplementationMixed ();

	void render ();
	void tick ();

private:
	unsigned int cutoff;
	unsigned int textureSize;

	static void renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers,
		Level* level, int textureSize, int maxRecursionDepth, int cutoff, Window* window);

	std::vector<unsigned int> portal1FrameBuffers;
	std::vector<unsigned int> portal1Textures;

	std::vector<unsigned int> portal2FrameBuffers;
	std::vector<unsigned int> portal2Textures;
};