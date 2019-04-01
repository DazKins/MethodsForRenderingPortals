#include "ImplementationFramebufferObjects.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "ModelLoader.h"

ImplementationFramebufferObjects::ImplementationFramebufferObjects (Input* input, Window* window,  int maxRecursionDepth) : Implementation (input, window, maxRecursionDepth)
{
	for (int i = 0; i < maxRecursionDepth - 1; i++)
	{
		auto portal1 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
		this->portal1FrameBuffers.push_back (std::get<0> (portal1));
		this->portal1Textures.push_back (std::get<1> (portal1));

		auto portal2 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
		this->portal2FrameBuffers.push_back (std::get<0> (portal2));
		this->portal2Textures.push_back (std::get<1> (portal2));
	}
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
	for (unsigned int i : this->portal1FrameBuffers)
		glDeleteFramebuffers (1, &i);

	for (unsigned int i : this->portal2FrameBuffers)
		glDeleteFramebuffers (1, &i);
}

glm::mat4 ImplementationFramebufferObjects::generateCustomProjection (glm::mat4 translationMatrix, Portal *inPortal, Portal *outPortal)
{
	glm::mat4 viewMatrix = getNewCameraView (translationMatrix, inPortal, outPortal);
	Shader::updateAllViewMatrices (viewMatrix);

	glm::mat4 viewModel = viewMatrix * outPortal->toWorld;

	glm::vec3 TL = viewModel * glm::vec4 (Portal::vertices[1], 1.0f);
	glm::vec3 BR = viewModel * glm::vec4 (Portal::vertices[3], 1.0f);

	glm::mat4 rotation = glm::inverse (glm::mat3 (inPortal->toWorld));

	TL = rotation * glm::vec4 (TL, 1.0f);
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

void ImplementationFramebufferObjects::renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal *outPortal,
	std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int maxRecursionDepth, int cutoff)
{
	std::vector<glm::mat4> translationMatrices;
	translationMatrices.push_back (translationMatrix);

	for (int i = 1; i < maxRecursionDepth; i++)
	{
		translationMatrices.push_back (getNewCameraView (translationMatrices[i - 1], inPortal, outPortal));
	}

	Shader::PORTAL_CLIP->setUniform ("portalPosition", outPortal->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", outPortal->getNormal ());

	for (int i = maxRecursionDepth - 1; i > cutoff; i--)
	{
		glBindFramebuffer (GL_FRAMEBUFFER, inPortalFrameBuffers[i - 1 - cutoff]);

		Shader::updateAllViewMatrices (translationMatrices[i]);

		Shader::updateAllProjectionMatrices (generateCustomProjection (translationMatrices[i - 1], inPortal, outPortal));

		glViewport (0, 0, ImplementationFramebufferObjects::portalTextureSize, ImplementationFramebufferObjects::portalTextureSize);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader::PORTAL_CLIP->bind ();
		level->render ();

		if (i < maxRecursionDepth - 1)
		{
			Shader::updateAllModelMatrices (inPortal->toWorld);
			glBindTexture (GL_TEXTURE_2D, inPortalTextures[i - cutoff]);
			Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
			inPortal->model->render ();
			Shader::updateAllModelMatrices (glm::mat4 (1.0f));
		}
	}
}

void ImplementationFramebufferObjects::renderFromPortalPerspective (glm::mat4 translationMatrix, Portal* inPortal, Portal *outPortal,
	std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers, Level* level, int maxRecursionDepth)
{
	renderFromPortalPerspective (translationMatrix, inPortal, outPortal, inPortalTextures, inPortalFrameBuffers, level, maxRecursionDepth, 0);
}

void ImplementationFramebufferObjects::render ()
{
	this->renderFromPortalPerspective (this->camera->getTranslationMatrix (), portal1, portal2, portal1Textures, portal1FrameBuffers, level, maxRecursionDepth);
	this->renderFromPortalPerspective (this->camera->getTranslationMatrix (), portal2, portal1, portal2Textures, portal2FrameBuffers, level, maxRecursionDepth);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	Shader::updateAllViewMatrices (this->camera->getViewMatrix ());

	Shader::updateAllProjectionMatrices (glm::perspective (45.0f, this->window->getAspectRatio (), 0.1f, 100.0f));
	glViewport (0, 0, this->window->getWidth (), this->window->getHeight ());

	Shader::DEFAULT->bind ();
	level->render ();

	Shader::updateAllModelMatrices (portal1->toWorld);
	glBindTexture (GL_TEXTURE_2D, this->portal1Textures[0]);
	Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
	portal1->model->render ();

	Shader::updateAllModelMatrices (portal2->toWorld);
	glBindTexture (GL_TEXTURE_2D, this->portal2Textures[0]);
	Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
	portal2->model->render ();

	Shader::updateAllModelMatrices (glm::mat4 (1.0f));
}

void ImplementationFramebufferObjects::tick ()
{
	Implementation::tick ();
}