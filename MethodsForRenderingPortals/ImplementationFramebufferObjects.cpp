#include "ImplementationFramebufferObjects.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"

ImplementationFramebufferObjects::ImplementationFramebufferObjects (Input* input, Window* window) : Implementation (input, window)
{
	auto portal1 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
	this->portal1FrameBuffer = std::get<0> (portal1);
	this->portal1Texture = std::get<1> (portal1);

	auto portal2 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
	this->portal2FrameBuffer = std::get<0> (portal2);
	this->portal2Texture = std::get<1> (portal2);

	float epsilon = 0.005f;

	this->portal1Position = glm::vec3 (0.0f, 0.0f, -(2.5f - epsilon));
	this->portal1Normal = glm::vec3 (0.0f, 0.0f, 1.0f);
	this->portal2Position = glm::vec3 (0.0f, 0.0f, 2.5f - epsilon);
	this->portal2Normal = glm::vec3 (0.0f, 0.0f, -1.0f);

	this->portal1 = new VAO ();
	this->portal2 = new VAO ();

	this->portal1Vertices[0] = glm::vec3 (-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position;
	this->portal1Vertices[1] = glm::vec3 (-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position;
	this->portal1Vertices[2] = glm::vec3 (PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position;
	this->portal1Vertices[3] = glm::vec3 (PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position;

	int v0 = this->portal1->setXYZ (portal1Vertices[0])->setUV (0.0f, 0.0f)->pushVertex ();
	int v1 = this->portal1->setXYZ (portal1Vertices[1])->setUV (0.0f, 1.0f)->pushVertex ();
	int v2 = this->portal1->setXYZ (portal1Vertices[2])->setUV (1.0f, 1.0f)->pushVertex ();
	int v3 = this->portal1->setXYZ (portal1Vertices[3])->setUV (1.0f, 0.0f)->pushVertex ();

	this->portal1->pushIndex (v0)->pushIndex (v1)->pushIndex (v2);
	this->portal1->pushIndex (v2)->pushIndex (v3)->pushIndex (v0);

	this->portal1->compile ();

	this->portal2Vertices[0] = glm::vec3 (-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position;
	this->portal2Vertices[1] = glm::vec3 (-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position;
	this->portal2Vertices[2] = glm::vec3 (PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position;
	this->portal2Vertices[3] = glm::vec3 (PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position;

	v0 = this->portal2->setXYZ (portal2Vertices[0])->setUV (0.0f, 0.0f)->pushVertex ();
	v1 = this->portal2->setXYZ (portal2Vertices[1])->setUV (0.0f, 1.0f)->pushVertex ();
	v2 = this->portal2->setXYZ (portal2Vertices[2])->setUV (1.0f, 1.0f)->pushVertex ();
	v3 = this->portal2->setXYZ (portal2Vertices[3])->setUV (1.0f, 0.0f)->pushVertex ();

	this->portal2->pushIndex (v0)->pushIndex (v1)->pushIndex (v2);
	this->portal2->pushIndex (v2)->pushIndex (v3)->pushIndex (v0);

	this->portal2->compile ();
}

std::tuple<unsigned int, unsigned int> ImplementationFramebufferObjects::createPortalFrameBuffer ()
{
	unsigned int fboId;

	glGenFramebuffers (1, &fboId);
	glBindFramebuffer (GL_FRAMEBUFFER, fboId);

	unsigned int textureId;
	glGenTextures (1, &textureId);
	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	unsigned int rbo;
	glGenRenderbuffers (1, &rbo);
	glBindRenderbuffer (GL_RENDERBUFFER, rbo);
	glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
	glBindRenderbuffer (GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Portal framebuffer is incomplete" << std::endl;

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	return std::make_tuple (fboId, textureId);
}

ImplementationFramebufferObjects::~ImplementationFramebufferObjects ()
{
	glDeleteFramebuffers (1, &this->portal1FrameBuffer);
	glDeleteFramebuffers (1, &this->portal2FrameBuffer);
}

void ImplementationFramebufferObjects::render ()
{
	Implementation::render ();

	// PORTAL 1

	glBindFramebuffer (GL_FRAMEBUFFER, this->portal1FrameBuffer);

	glm::vec3 position = this->camera->getPosition ();
	this->camera->setPosition (position - this->portal1Position + this->portal2Position);

	glm::mat4 viewMatrix = this->camera->getTranslationMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	glm::vec3 portalViewPoints[4];

	for (int i = 0; i < 4; i++)
	{
		portalViewPoints[i] = viewMatrix * glm::vec4 (portal2Vertices[i], 1.0f);
	}

	glm::vec3 TL = portalViewPoints[1];
	glm::vec3 BR = portalViewPoints[3];

	float common_z = abs(TL.z);
	float near_z = 0.1f;

	float left = TL.x / common_z * near_z;
	float right = BR.x / common_z * near_z;
	float bottom = BR.y / common_z * near_z;
	float top = TL.y / common_z * near_z;

	glm::mat4 frustumProjection = glm::frustum (left, right, bottom, top, near_z, 100.0f);

	Shader::updateAllProjectionMatrices (frustumProjection);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal2Normal);

	Shader::PORTAL_CLIP->bind ();

	glViewport (0, 0, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
	glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	level->render ();

	this->camera->setPosition (position);

	// PORTAL 2

	glBindFramebuffer (GL_FRAMEBUFFER, this->portal2FrameBuffer);

	position = this->camera->getPosition ();
	this->camera->setPosition (position - this->portal2Position + this->portal1Position);

	viewMatrix = this->camera->getTranslationMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	for (int i = 0; i < 4; i++)
	{
		portalViewPoints[i] = viewMatrix * glm::vec4 (portal1Vertices[i], 1.0f);
	}

	TL = portalViewPoints[1];
	glm::vec3 BL = portalViewPoints[0];
	BR = portalViewPoints[3];

	float dz = BL.z - TL.z;
	float dx = BL.x - TL.x;
	
	float rotationY = 0;

	if (dz == 0 && dx == 0)
	{
		if (BL.z > 0)
			rotationY = M_PI;
	}
	else
		rotationY = atan2(dz, dx);

	glm::mat4 rotation = glm::rotate (glm::mat4 (1), rotationY, glm::vec3 (0.0f, 1.0f, 0.0f));

	TL = rotation * glm::vec4 (TL, 1.0f);
	BL = rotation * glm::vec4 (BL, 1.0f);
	BR = rotation * glm::vec4 (BR, 1.0f);

	common_z = abs (TL.z);
	near_z = 0.1f;

	left = TL.x / common_z * near_z;
	right = BR.x / common_z * near_z;
	bottom = BR.y / common_z * near_z;
	top = TL.y / common_z * near_z;

	frustumProjection = glm::frustum (left, right, bottom, top, near_z, 100.0f);

	Shader::updateAllProjectionMatrices (frustumProjection * rotation);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal1Normal);

	Shader::PORTAL_CLIP->bind ();

	glViewport (0, 0, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
	glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	level->render ();

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	// MAIN SCENE

	this->camera->setPosition (position);

	viewMatrix = this->camera->getViewMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	Shader::DEFAULT->updateAllProjectionMatrices (glm::perspective (45.0f, this->window->getAspectRatio (), 0.1f, 100.0f));
	glViewport (0, 0, this->window->getWidth (), this->window->getHeight ());

	Shader::DEFAULT->bind ();
	level->render ();

	glBindTexture (GL_TEXTURE_2D, this->portal1Texture);
	portal1->render ();

	glBindTexture (GL_TEXTURE_2D, this->portal2Texture);
	portal2->render ();
}

void ImplementationFramebufferObjects::tick ()
{
	Implementation::tick ();
}