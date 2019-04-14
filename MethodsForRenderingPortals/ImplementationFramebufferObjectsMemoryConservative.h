#pragma once

#include "Implementation.h"
#include "ImplementationMixed.h"

#include <tuple>
#include <vector>

class ImplementationFramebufferObjectsMemoryConservative : public Implementation
{
public:
	ImplementationFramebufferObjectsMemoryConservative (Input* input, Window* window, int textureSize, int maxRecursionDepth, bool manualCamera);
	~ImplementationFramebufferObjectsMemoryConservative ();

	void render ();
	void tick ();

private:
	unsigned int* portalFrameBuffers;
	unsigned int* portalTextures;

	unsigned int textureSize;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer (int textureSize);

	static glm::mat4 generateCustomProjection (glm::mat4 oldViewMatrix, glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		unsigned int* inPortalTextures, unsigned int* inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff,
		glm::mat4* viewOperators);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		unsigned int* inPortalTextures, unsigned int* inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth,
		glm::mat4* viewOperators);

	friend class ImplementationMixed;
};

