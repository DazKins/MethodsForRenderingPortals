#include "ImplementationStencilBuffer.h"

ImplementationStencilBuffer::ImplementationStencilBuffer (Input *input, Window* window) : Implementation (input, window)
{
	float epsilon = 0.0005f;

	this->portal1Position = glm::vec3 (0.0f, 0.0f, -(2.5f - epsilon));
	this->portal1Normal = glm::vec3 (0.0f, 0.0f, 1.0f);
	this->portal2Position = glm::vec3 (0.0f, 0.0f, 2.5f - epsilon);
	this->portal2Normal = glm::vec3 (0.0f, 0.0f, -1.0f);

	this->portal1 = new VAO ();
	this->portal2 = new VAO ();

	int v0 = this->portal1->setXYZ (glm::vec3 (-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex ();
	int v1 = this->portal1->setXYZ (glm::vec3 (-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex ();
	int v2 = this->portal1->setXYZ (glm::vec3 (PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex ();
	int v3 = this->portal1->setXYZ (glm::vec3 (PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex ();

	this->portal1->pushIndex (v0)->pushIndex (v1)->pushIndex (v2);
	this->portal1->pushIndex (v2)->pushIndex (v3)->pushIndex (v0);

	this->portal1->compile();

	v0 = this->portal2->setXYZ (glm::vec3 (-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v1 = this->portal2->setXYZ (glm::vec3 (-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v2 = this->portal2->setXYZ (glm::vec3 (PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v3 = this->portal2->setXYZ (glm::vec3 (PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();

	this->portal2->pushIndex (v0)->pushIndex (v1)->pushIndex (v2);
	this->portal2->pushIndex (v2)->pushIndex (v3)->pushIndex (v0);

	this->portal2->compile ();
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
	this->portal1->render ();

	glStencilFunc (GL_ALWAYS, 2, 0xFF);
	Shader::PORTAL_STENCIL_BUFFER->bind ();
	this->portal2->render ();

	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	// PORTAL 1

	glm::vec3 position = cam->getPosition ();
	cam->setPosition (position - portal1Position + portal2Position);
	viewMatrix = cam->getViewMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc(GL_EQUAL, 1, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal2Normal);
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();

	// PORTAL 2

	cam->setPosition (position - portal2Position + portal1Position);
	viewMatrix = cam->getViewMatrix ();
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc(GL_EQUAL, 2, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", portal1Normal);
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render ();

	Shader::DEFAULT->bind ();
	glDisable (GL_STENCIL_TEST);
	cam->setPosition (position);
}