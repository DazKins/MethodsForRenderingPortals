#include "ImplementationStencilBuffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ImplementationStencilBuffer::ImplementationStencilBuffer (Input *input, Window *window) : Implementation (input, window)
{
	float epsilon = 0.0005f;

	this->portal1 = new Portal ();
	this->portal2 = new Portal ();

	glm::vec3 portal1Position = glm::vec3 (0.0f, 0.0f, -(2.5f - epsilon));
	glm::vec3 portal2Position = glm::vec3 (-2.0f, 0.0f, 2.0f);

	this->portal1->generatePortalMesh ();
	this->portal2->generatePortalMesh ();

	this->portal1->toWorld = glm::translate (glm::mat4 (1.0f), portal1Position);

	this->portal2->toWorld = glm::translate (glm::mat4 (1.0f), portal2Position) * glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f))
		* glm::rotate (glm::mat4 (1.0f), glm::radians (-45.0f), glm::vec3 (0.0f, 1.0f, 0.0f));
}

ImplementationStencilBuffer::~ImplementationStencilBuffer () { }

void ImplementationStencilBuffer::render ()
{
	Implementation::render ();

	renderFromPerspective (this->camera);
}

void ImplementationStencilBuffer::tick ()
{
	Implementation::tick ();
}

void ImplementationStencilBuffer::renderFromPerspective (Camera* cam)
{
	glm::mat4 viewMatrix = cam->getViewMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	glEnable (GL_STENCIL_TEST);
	glClear (GL_STENCIL_BUFFER_BIT);

	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	glStencilFunc (GL_EQUAL, 0, 0xFF);
	Shader::DEFAULT->bind ();
	level->render ();

	glStencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc (GL_ALWAYS, 1, 0xFF);
	Shader::PORTAL_STENCIL_BUFFER->bind ();
	Shader::updateAllModelMatrices (portal1->toWorld);
	this->portal1->model->render ();

	glStencilFunc (GL_ALWAYS, 2, 0xFF);
	Shader::PORTAL_STENCIL_BUFFER->bind ();
	Shader::updateAllModelMatrices (portal2->toWorld);
	this->portal2->model->render ();

	Shader::updateAllModelMatrices (glm::mat4 (1.0f));

	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	// PORTAL 1
	viewMatrix = getNewCameraView (cam->getViewMatrix (), portal1, portal2);
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc (GL_EQUAL, 1, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal2->getNormal ());
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();

	// PORTAL 2

	viewMatrix = getNewCameraView (cam->getViewMatrix (), portal2, portal1);
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc (GL_EQUAL, 2, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1->getPosition ());
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal1->getNormal ());
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();

	Shader::DEFAULT->bind ();
	glDisable (GL_STENCIL_TEST);
}