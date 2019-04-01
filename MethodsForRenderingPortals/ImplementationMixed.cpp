#include "ImplementationMixed.h"
#include "ImplementationFramebufferObjects.h"
#include "Shader.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

ImplementationMixed::ImplementationMixed (Input* input, Window* window, int maxRecursionDepth, int cutoff) : Implementation(input, window, maxRecursionDepth)
{
	this->cutoff = cutoff;

	for (int i = cutoff; i < maxRecursionDepth; i++)
	{
		auto portal1 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
		this->portal1FrameBuffers.push_back (std::get<0> (portal1));
		this->portal1Textures.push_back (std::get<1> (portal1));

		auto portal2 = ImplementationFramebufferObjects::createPortalFrameBuffer ();
		this->portal2FrameBuffers.push_back (std::get<0> (portal2));
		this->portal2Textures.push_back (std::get<1> (portal2));
	}
}

ImplementationMixed::~ImplementationMixed ()
{
	for (unsigned int i : this->portal1FrameBuffers)
		glDeleteFramebuffers (1, &i);

	for (unsigned int i : this->portal2FrameBuffers)
		glDeleteFramebuffers (1, &i);
}

void ImplementationMixed::renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers,
	Level* level, int maxRecursionDepth, int cutoff, Window* window)
{
	ImplementationFramebufferObjects::renderFromPortalPerspective (camera->getTranslationMatrix (), inPortal, outPortal, inPortalTextures, inPortalFrameBuffers, level, maxRecursionDepth, cutoff);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	Shader::updateAllViewMatrices (camera->getViewMatrix ());

	Shader::updateAllProjectionMatrices (glm::perspective (45.0f, window->getAspectRatio (), 0.1f, 100.0f));
	glViewport (0, 0, window->getWidth (), window->getHeight ());

	std::vector<glm::mat4> viewMatrices;
	viewMatrices.push_back (camera->getViewMatrix ());

	for (int i = 1; i < maxRecursionDepth; i++)
	{
		viewMatrices.push_back (getNewCameraView (viewMatrices[i - 1], inPortal, outPortal));
	}

	Shader::PORTAL_CLIP->setUniform ("portalPosition", outPortal->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", outPortal->getNormal ());

	for (int i = 0; i <= cutoff; i++)
	{
		Shader::updateAllViewMatrices (viewMatrices[i]);

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP->bind ();

		if (i == 0)
			Shader::DEFAULT->bind ();

		level->render ();

		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);

		Shader::updateAllModelMatrices (inPortal->toWorld);
		if (i == cutoff)
		{
			glBindTexture (GL_TEXTURE_2D, inPortalTextures[0]);
			Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
			inPortal->model->render ();
		}
		else
		{
			Shader::PORTAL_STENCIL_BUFFER->bind ();
			inPortal->model->render ();
		}
		Shader::updateAllModelMatrices (glm::mat4 (1.0f));

		glClear (GL_DEPTH_BUFFER_BIT);
	}
}

void ImplementationMixed::render ()
{
	Implementation::render ();

	glEnable (GL_STENCIL_TEST);
	glClear (GL_STENCIL_BUFFER_BIT);

	this->renderFromPerspective (this->camera, portal2, portal1, portal2Textures, portal2FrameBuffers, level, maxRecursionDepth, cutoff, this->window);
	this->renderFromPerspective (this->camera, portal1, portal2, portal1Textures, portal1FrameBuffers, level, maxRecursionDepth, cutoff, this->window);
}

void ImplementationMixed::tick ()
{
	Implementation::tick ();
}