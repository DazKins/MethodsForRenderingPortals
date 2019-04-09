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
	std::vector<unsigned int> portal1FrameBuffers;
	std::vector<unsigned int> portal1Textures;

	std::vector<unsigned int> portal2FrameBuffers;
	std::vector<unsigned int> portal2Textures;

	unsigned int textureSize;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer (int textureSize);

	static glm::mat4 generateCustomProjection (glm::mat4 oldViewMatrix, glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff,
		std::vector<glm::mat4> viewOperators);

	static void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal* outPortal,
		std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth,
		std::vector<glm::mat4> viewOperators);

	friend class ImplementationMixed;
};

