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
	glm::vec3 portal1Position;
	glm::vec3 portal1Normal;

	glm::vec3 portal2Position;
	glm::vec3 portal2Normal;

	VAO* portal1;
	VAO* portal2;

	glm::vec3 portal1Vertices[4];
	glm::vec3 portal2Vertices[4];

	unsigned int portal1FrameBuffer;
	unsigned int portal1Texture;

	unsigned int portal2FrameBuffer;
	unsigned int portal2Texture;

	static std::tuple<unsigned int, unsigned int> createPortalFrameBuffer ();

	static const unsigned int portalTextureSize = 400;
};

