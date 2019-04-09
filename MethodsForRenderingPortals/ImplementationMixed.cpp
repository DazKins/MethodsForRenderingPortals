#include "ImplementationMixed.h"
#include "ImplementationFramebufferObjects.h"
#include "Shader.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

ImplementationMixed::ImplementationMixed (Input* input, Window* window, int textureSize, int maxRecursionDepth, int cutoff, bool manualCamera)
	: Implementation(input, window, maxRecursionDepth, manualCamera)
{
	this->cutoff = cutoff;
	this->textureSize = textureSize;

	for (int i = cutoff; i < maxRecursionDepth * 2; i++)
	{
		auto portal = ImplementationFramebufferObjects::createPortalFrameBuffer (textureSize);
		this->portalFrameBuffers.push_back (std::get<0> (portal));
		this->portalTextures.push_back (std::get<1> (portal));
	}
}

ImplementationMixed::~ImplementationMixed ()
{
	for (unsigned int i : this->portalFrameBuffers)
		glDeleteFramebuffers (1, &i);
}

void ImplementationMixed::renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, std::vector<unsigned int> inPortalTextures, 
	std::vector<unsigned int> inPortalFrameBuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff, Window* window, std::vector<glm::mat4> viewOperators)
{
	glDisable (GL_STENCIL_TEST);

	ImplementationFramebufferObjects::renderFromPortalPerspective (camera->getTranslationMatrix (), inPortal, outPortal, inPortalTextures,
		inPortalFrameBuffers, level, textureSize, maxRecursionDepth, cutoff, viewOperators);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	Shader::updateAllProjectionMatrices (glm::perspective (45.0f, window->getAspectRatio (), 0.001f, 100.0f));
	glViewport (0, 0, window->getWidth (), window->getHeight ());

	Shader::PORTAL_CLIP->setUniform ("portalPosition", outPortal->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", outPortal->getNormal ());

	glEnable (GL_STENCIL_TEST);

	for (int i = 0; i <= cutoff; i++)
	{
		Shader::updateAllViewMatrices (camera->getViewMatrix() * viewOperators[i]);

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP->bind ();

		if (i == 0)
			Shader::DEFAULT->bind ();

		level->render ();

		Shader::updateAllModelMatrices (inPortal->getToWorld ());
		if (i == cutoff)
		{
			glBindTexture (GL_TEXTURE_2D, inPortalTextures[0]);
			Shader::PORTAL_FRAMEBUFFER_OBJECT->bind ();
		}
		else
		{
			glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);
			Shader::PORTAL_STENCIL_BUFFER->bind ();
		}
		inPortal->model->render ();
		Shader::updateAllModelMatrices (glm::mat4 (1.0f));
		glClear (GL_DEPTH_BUFFER_BIT);
	}

	glBindTexture (GL_TEXTURE_2D, 0);
}

void ImplementationMixed::render ()
{
	Implementation::render ();

	glClear (GL_STENCIL_BUFFER_BIT);

	this->renderFromPerspective (camera, portal2, portal1, portalTextures, portalFrameBuffers, level, textureSize,
		maxRecursionDepth, cutoff, window, portal2ViewOperators);

	// Reusing framebuffers/textures seems like a good idea
	this->renderFromPerspective (camera, portal1, portal2, portalTextures, portalFrameBuffers, level, textureSize,
		maxRecursionDepth, cutoff, window, portal1ViewOperators);
}

void ImplementationMixed::tick ()
{
	Implementation::tick ();
}