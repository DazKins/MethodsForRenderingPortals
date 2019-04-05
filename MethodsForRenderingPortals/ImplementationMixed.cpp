#include "ImplementationMixed.h"
#include "ImplementationFramebufferObjects.h"
#include "Shader.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

ImplementationMixed::ImplementationMixed (Input* input, Window* window, int textureSize, int maxRecursionDepth, int cutoff) : Implementation(input, window, maxRecursionDepth)
{
	this->cutoff = cutoff;
	this->textureSize = textureSize;

	for (int i = cutoff; i < maxRecursionDepth; i++)
	{
		auto portal2 = ImplementationFramebufferObjects::createPortalFrameBuffer (textureSize);
		this->portalFrameBuffers.push_back (std::get<0> (portal2));
		this->portalTextures.push_back (std::get<1> (portal2));
	}
}

ImplementationMixed::~ImplementationMixed ()
{
	for (unsigned int i : this->portalFrameBuffers)
		glDeleteFramebuffers (1, &i);

	for (unsigned int i : this->portalFrameBuffers)
		glDeleteFramebuffers (1, &i);
}

void ImplementationMixed::renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, std::vector<unsigned int> inPortalTextures, std::vector<unsigned int> inPortalFrameBuffers,
	Level* level, int textureSize, int maxRecursionDepth, int cutoff, Window* window)
{
	glDisable (GL_STENCIL_TEST);

	ImplementationFramebufferObjects::renderFromPortalPerspective (camera->getTranslationMatrix (), inPortal, outPortal, inPortalTextures, inPortalFrameBuffers, level, textureSize, maxRecursionDepth, cutoff);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	Shader::updateAllViewMatrices (camera->getViewMatrix ());

	Shader::updateAllProjectionMatrices (glm::perspective (45.0f, window->getAspectRatio (), 0.001f, 100.0f));
	glViewport (0, 0, window->getWidth (), window->getHeight ());

	std::vector<glm::mat4> viewMatrices;
	viewMatrices.push_back (camera->getViewMatrix ());

	for (int i = 1; i < maxRecursionDepth; i++)
	{
		viewMatrices.push_back (getNewCameraView (viewMatrices[i - 1], inPortal, outPortal));
	}

	Shader::PORTAL_CLIP->setUniform ("portalPosition", outPortal->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", outPortal->getNormal ());

	glEnable (GL_STENCIL_TEST);

	for (int i = 0; i <= cutoff; i++)
	{
		Shader::updateAllViewMatrices (viewMatrices[i]);

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP->bind ();

		if (i == 0)
			Shader::DEFAULT->bind ();

		level->render ();

		Shader::updateAllModelMatrices (inPortal->toWorld);
		if (i == cutoff)
		{
			glStencilFunc (GL_EQUAL, i, 0xFF);
			Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
			glBindTexture (GL_TEXTURE_2D, inPortalTextures[0]);
			inPortal->model->render ();
		}
		else
		{
			glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);
			Shader::PORTAL_STENCIL_BUFFER->bind ();
			inPortal->model->render ();
			glClear (GL_DEPTH_BUFFER_BIT);
		}

		Shader::updateAllModelMatrices (glm::mat4 (1.0f));
	}

	glBindTexture (GL_TEXTURE_2D, 0);
}

void ImplementationMixed::render ()
{
	Implementation::render ();

	glClear (GL_STENCIL_BUFFER_BIT);

	this->renderFromPerspective (this->camera, portal2, portal1, portalTextures, portalFrameBuffers, level, textureSize, maxRecursionDepth, cutoff, this->window);

	// Reusing framebuffers/textures seems like a good idea
	this->renderFromPerspective (this->camera, portal1, portal2, portalTextures, portalFrameBuffers, level, textureSize, maxRecursionDepth, cutoff, this->window);
}

void ImplementationMixed::tick ()
{
	Implementation::tick ();
}