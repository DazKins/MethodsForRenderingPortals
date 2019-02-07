#include "ImplementationStencilBuffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ImplementationStencilBuffer::ImplementationStencilBuffer (Input *input, Window *window) : Implementation (input, window)
{
}

ImplementationStencilBuffer::~ImplementationStencilBuffer () { }

void ImplementationStencilBuffer::render ()
{
	Implementation::render ();

	glEnable (GL_STENCIL_TEST);
	glClear (GL_STENCIL_BUFFER_BIT);

	renderFromPerspective (this->camera->getViewMatrix ());
}

void ImplementationStencilBuffer::tick ()
{
	Implementation::tick ();
}

void ImplementationStencilBuffer::renderFromPerspective (glm::mat4 viewMatrix)
{
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	glStencilFunc (GL_EQUAL, 0, 0xFF);
	Shader::DEFAULT->bind ();
	level->render ();

	glStencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc (GL_ALWAYS, 1, 0xFF);
	Shader::updateAllModelMatrices (portal1->toWorld);
	Shader::PORTAL_STENCIL_BUFFER->bind ();
	this->portal1->model->render ();

	glStencilFunc (GL_ALWAYS, 2, 0xFF);
	Shader::updateAllModelMatrices (portal2->toWorld);
	Shader::PORTAL_STENCIL_BUFFER->bind ();
	this->portal2->model->render ();

	Shader::updateAllModelMatrices (glm::mat4 (1.0f));

	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	// PORTAL 1
	glm::mat4 newViewMatrix = getNewCameraView (viewMatrix, portal1, portal2);
	Shader::updateAllViewMatrices (newViewMatrix);

	glStencilFunc (GL_EQUAL, 1, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal2->getNormal ());
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();

	// PORTAL 2

	newViewMatrix = getNewCameraView (viewMatrix, portal2, portal1);
	Shader::updateAllViewMatrices (newViewMatrix);

	glStencilFunc (GL_EQUAL, 2, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal1->getNormal ());
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();
}