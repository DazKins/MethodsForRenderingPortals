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

	renderPortalView (this->camera->getViewMatrix (), portal1, portal2, level, maxRecursionDepth, portal1ViewOperators);
	renderPortalView (this->camera->getViewMatrix (), portal2, portal1, level, maxRecursionDepth, portal2ViewOperators);
}

void ImplementationStencilBuffer::tick ()
{
	Implementation::tick ();
}

void ImplementationStencilBuffer::renderPortalView (glm::mat4 viewMatrix, Portal *inPortal, Portal *outPortal, Level *level, int maxRecursionDepth, glm::mat4* viewOperators)
{
	Shader::PORTAL_CLIP.bind ();
	Shader::setUniform ("portalPosition", outPortal->getPosition ());
	Shader::setUniform ("portalNormal", outPortal->getNormal ());

	for (int i = 0; i < maxRecursionDepth; i++)
	{
		glm::mat4 thisViewMatrix;

		if (i == 0)
			thisViewMatrix = viewMatrix;
		else
			thisViewMatrix = viewMatrix * viewOperators[i];

		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc (GL_EQUAL, i, 0xFF);

		Shader::PORTAL_CLIP.bind ();

		if (i == 0)
			Shader::DEFAULT.bind ();

		Shader::setUniform ("viewMatrix", thisViewMatrix);

		level->render ();

		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);

		Shader::PORTAL_STENCIL_BUFFER.bind ();
		Shader::setUniform ("viewMatrix", thisViewMatrix);
		Shader::setUniform ("modelMatrix", inPortal->getToWorld ());
		inPortal->model->render ();

		glClear (GL_DEPTH_BUFFER_BIT);
	}
}