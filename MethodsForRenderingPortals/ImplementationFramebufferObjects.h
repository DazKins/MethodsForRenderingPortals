#pragma once

#include "Implementation.h"
#include "ImplementationMixed.h"

#include <tuple>
#include <vector>

class ImplementationFramebufferObjects : public Implementation
{
public:
	ImplementationFramebufferObjects (Input* input, Window* window, int textureSize, int maxRecursionDepth, bool manualCamera);
	~ImplementationFramebufferObjects ();

	void render ();
	void tick ();

private:
	unsigned int * portal1FrameBuffers;
	unsigned int* portal1Textures;
	unsigned int* portal2FrameBuffers;
	unsigned int* portal2Textures;

	unsigned int textureSize;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer (int textureSize);

	static glm::mat4 generateCustomProjection (glm::mat4 oldViewMatrix, glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		unsigned int * inPortalTextures, unsigned int* inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff,
		glm::mat4 * viewOperators);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		unsigned int* inPortalTextures, unsigned int* inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth,
		glm::mat4* viewOperators);

	friend class ImplementationMixed;
};

