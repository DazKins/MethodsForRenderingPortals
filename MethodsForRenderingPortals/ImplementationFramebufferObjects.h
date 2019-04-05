#pragma once

#include "Implementation.h"
#include "ImplementationMixed.h"

#include <tuple>
#include <vector>

class ImplementationFramebufferObjects : public Implementation
{
public:
	ImplementationFramebufferObjects (Input* input, Window* window, int textureSize, int maxRecursionDepth);
	~ImplementationFramebufferObjects ();

	void render ();
	void tick ();

private:
	std::vector<unsigned int> portal1FrameBuffers;
	std::vector<unsigned int> portal1Textures;

	std::vector<unsigned int> portal2FrameBuffers;
	std::vector<unsigned int> portal2Textures;

	unsigned int textureSize;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer (int textureSize);

	static glm::mat4 generateCustomProjection (glm::mat4 translationMatrix, Portal *inPortal, Portal *outPortal);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal *outPortal,
		std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal *outPortal,
		std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth);

	friend class ImplementationMixed;
};

