#pragma once

#include "Implementation.h"

#include <tuple>
#include <vector>

class ImplementationFramebufferObjects : public Implementation
{
public:
	ImplementationFramebufferObjects (Input* input, Window* window);
	~ImplementationFramebufferObjects ();

	void render ();
	void tick ();

private:
	std::vector<unsigned int> portal1FrameBuffers;
	std::vector<unsigned int> portal1Textures;

	std::vector<unsigned int> portal2FrameBuffers;
	std::vector<unsigned int> portal2Textures;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer ();

	static glm::mat4 generateCustomProjection (glm::mat4 translationMatrix, Portal *inPortal, Portal *outPortal);

	void renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal *outPortal,
		std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers);

	static const unsigned int portalTextureSize = 400;
};

