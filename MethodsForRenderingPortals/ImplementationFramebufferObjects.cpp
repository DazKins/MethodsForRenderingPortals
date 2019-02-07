#include "ImplementationFramebufferObjects.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "ModelLoader.h"

ImplementationFramebufferObjects::ImplementationFramebufferObjects (Input* input, Window* window) : Implementation (input, window)
{
	auto portal1 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
	this->portal1FrameBuffer = std::get<0> (portal1);
	this->portal1Texture = std::get<1> (portal1);

	auto portal2 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
	this->portal2FrameBuffer = std::get<0> (portal2);
	this->portal2Texture = std::get<1> (portal2);
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

glm::mat4 ImplementationFramebufferObjects::generateCustomProjection (Camera *camera, Portal *inPortal, Portal *outPortal)
{
	glm::mat4 viewMatrix = getNewCameraView (camera->getTranslationMatrix (), inPortal, outPortal);
	Shader::updateAllViewMatrices (viewMatrix);

	glm::vec3 portalViewPoints[4];

	for (int i = 0; i < 4; i++)
	{
		portalViewPoints[i] = viewMatrix * outPortal->toWorld * glm::vec4 (Portal::vertices[i], 1.0f);
	}

	glm::vec3 TL = portalViewPoints[1];
	glm::vec3 BR = portalViewPoints[3];
	glm::vec3 BL = portalViewPoints[0];

	glm::mat4 rotation = glm::mat4 (glm::inverse (glm::mat3 (inPortal->toWorld)));

	TL = rotation * glm::vec4 (TL, 1.0f);
	BL = rotation * glm::vec4 (BL, 1.0f);
	BR = rotation * glm::vec4 (BR, 1.0f);

	float common_z = abs (TL.z);
	float near_z = 0.1f;

	float left = TL.x / common_z * near_z;
	float right = BR.x / common_z * near_z;
	float bottom = BR.y / common_z * near_z;
	float top = TL.y / common_z * near_z;

	glm::mat4 frustumProjection = glm::frustum (right, left, bottom, top, near_z, 100.0f);

	return frustumProjection * rotation;
}

void ImplementationFramebufferObjects::render ()
{
	Implementation::render ();

	// PORTAL 1

	glBindFramebuffer (GL_FRAMEBUFFER, this->portal1FrameBuffer);

	Shader::updateAllProjectionMatrices (generateCustomProjection (this->camera, portal1, portal2));

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal2->getNormal ());

	Shader::PORTAL_CLIP->bind ();

	glViewport (0, 0, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	level->render ();

	// PORTAL 2

	glBindFramebuffer (GL_FRAMEBUFFER, this->portal2FrameBuffer);

	Shader::updateAllProjectionMatrices (generateCustomProjection (this->camera, portal2, portal1));

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal1->getNormal ());

	Shader::PORTAL_CLIP->bind ();

	glViewport (0, 0, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	level->render ();

	// MAIN SCENE

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	Shader::updateAllViewMatrices (this->camera->getViewMatrix ());

	Shader::updateAllProjectionMatrices (glm::perspective (45.0f, this->window->getAspectRatio (), 0.1f, 100.0f));
	glViewport (0, 0, this->window->getWidth (), this->window->getHeight ());

	Shader::DEFAULT->bind ();
	level->render ();

	Shader::updateAllModelMatrices (portal1->toWorld);
	glBindTexture (GL_TEXTURE_2D, this->portal1Texture);
	Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
	portal1->model->render ();

	Shader::updateAllModelMatrices (portal2->toWorld);
	glBindTexture (GL_TEXTURE_2D, this->portal2Texture);
	Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
	portal2->model->render ();

	Shader::updateAllModelMatrices (glm::mat4 (1.0f));
}

void ImplementationFramebufferObjects::tick ()
{
	Implementation::tick ();
}