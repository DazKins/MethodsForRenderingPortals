#pragma once

#include "Implementation.h"

#include <tuple>

class ImplementationFramebufferObjects : public Implementation
{
public:
	ImplementationFramebufferObjects (Input* input, Window* window);
	~ImplementationFramebufferObjects ();

	void render ();
	void tick ();

private:
	unsigned int portal1FrameBuffer;
	unsigned int portal1Texture;

	unsigned int portal2FrameBuffer;
	unsigned int portal2Texture;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer ();

	static glm::mat4 generateCustomProjection (Camera *camera, Portal *inPortal, Portal *outPortal);

	static const unsigned int portalTextureSize = 100;
};

