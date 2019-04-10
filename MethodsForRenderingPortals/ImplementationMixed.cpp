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

	this->portalFrameBuffers = (unsigned int*) malloc (sizeof (unsigned int) * (maxRecursionDepth - cutoff));
	this->portalTextures = (unsigned int*) malloc (sizeof (unsigned int) * (maxRecursionDepth - cutoff));

	for (int i = cutoff; i < maxRecursionDepth; i++)
	{
		auto portal = ImplementationFramebufferObjects::createPortalFrameBuffer (textureSize);
		this->portalFrameBuffers[i - cutoff] = std::get<0> (portal);
		this->portalTextures[i - cutoff] = std::get<1> (portal);
	}
}

ImplementationMixed::~ImplementationMixed ()
{
	//for (unsigned int i : this->portalFrameBuffers)
	//	glDeleteFramebuffers (1, &i);
}

glm::mat4 ImplementationMixed::defaultProjection;

void ImplementationMixed::renderFromPerspective (Camera* camera, Portal* inPortal, Portal* outPortal, unsigned int* portalTextures,
	unsigned int* portalFramebuffers, Level* level, int textureSize, int maxRecursionDepth, int cutoff, Window* window, glm::mat4* viewOperators)
{
	glDisable (GL_STENCIL_TEST);

	ImplementationFramebufferObjects::renderFromPortalPerspective (camera->getTranslationMatrix (), inPortal, outPortal, portalTextures,
		portalFramebuffers, level, textureSize, maxRecursionDepth, cutoff, viewOperators);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glViewport (0, 0, window->getWidth (), window->getHeight ());

	Shader::PORTAL_FRAMEBUFFER_OBJECT.bind ();
	Shader::setUniform ("projectionMatrix", defaultProjection);

	Shader::PORTAL_CLIP.bind ();
	Shader::setUniform ("projectionMatrix", defaultProjection);
	Shader::setUniform ("portalPosition", outPortal->getPosition ());
	Shader::setUniform ("portalNormal", outPortal->getNormal ());

	glEnable (GL_STENCIL_TEST);

	for (int i = 0; i <= cutoff; i++)
	{
		glm::mat4 thisViewMatrix;

		if (i == 0)
			thisViewMatrix = camera->getViewMatrix ();
		else
			thisViewMatrix = camera->getViewMatrix () * viewOperators[i];

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP.bind ();

		if (i == 0)
			Shader::DEFAULT.bind ();

		Shader::setUniform ("viewMatrix", thisViewMatrix);

		level->render ();

		if (i == cutoff)
		{
			glBindTexture (GL_TEXTURE_2D, portalTextures[1]);
			Shader::PORTAL_FRAMEBUFFER_OBJECT.bind ();
		}
		else
		{
			glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);
			Shader::PORTAL_STENCIL_BUFFER.bind ();
		}

		Shader::setUniform ("viewMatrix", thisViewMatrix);
		Shader::setUniform ("modelMatrix", inPortal->getToWorld ());

		inPortal->model->render ();

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