#include "ImplementationStencilBuffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

ImplementationStencilBuffer::ImplementationStencilBuffer (Input *input, Window *window, int maxRecursionDepth, bool manualCamera) 
	: Implementation (input, window, maxRecursionDepth, manualCamera)
{
}

ImplementationStencilBuffer::~ImplementationStencilBuffer () { }

void ImplementationStencilBuffer::render ()
{
	Implementation::render ();

	glEnable (GL_STENCIL_TEST);
	glClear (GL_STENCIL_BUFFER_BIT);

	renderPortalView (this->camera->getViewMatrix (), portal1, portal2, level, maxRecursionDepth);
	renderPortalView (this->camera->getViewMatrix (), portal2, portal1, level, maxRecursionDepth);
}

void ImplementationStencilBuffer::tick ()
{
	Implementation::tick ();
}

void ImplementationStencilBuffer::renderPortalView (glm::mat4 viewMatrix, Portal *inPortal, Portal *outPortal, Level *level, int maxRecursionDepth)
{
	std::vector<glm::mat4> viewMatrices;
	viewMatrices.push_back (viewMatrix);

	for (int i = 1; i < maxRecursionDepth; i++)
	{
		viewMatrices.push_back (getNewCameraView (viewMatrices[i - 1], inPortal, outPortal));
	}

	Shader::PORTAL_CLIP->setUniform ("portalPosition", outPortal->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", outPortal->getNormal ());

	for (int i = 0; i < maxRecursionDepth; i++)
	{
		Shader::updateAllViewMatrices (viewMatrices[i]);

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP->bind ();

		if (i == 0)
			Shader::DEFAULT->bind ();

		level->render ();

		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);

		Shader::updateAllModelMatrices (inPortal->getToWorld ());
		Shader::PORTAL_STENCIL_BUFFER->bind ();
		inPortal->model->render ();
		Shader::updateAllModelMatrices (glm::mat4 (1.0f));

		glClear (GL_DEPTH_BUFFER_BIT);
	}
}