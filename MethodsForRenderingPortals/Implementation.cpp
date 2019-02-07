#include "Implementation.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"

float Implementation::PORTAL_SIZE = 0.5f;

Implementation::Implementation (Input* input, Window* window)
{
	this->window = window;
	this->input = input;

	this->camera = new Camera (this->window);
	this->player = new Player (this->input, this->window, this->camera);

	this->level = new Level (5.0f, 1.0f, 5.0f);

	glm::mat4 projectionMatrix = glm::perspective (45.0f, this->window->getAspectRatio (), 0.1f, 100.0f);
	Shader::updateAllProjectionMatrices (projectionMatrix);
	float epsilon = 0.0005f;

	this->portal1 = new Portal ();
	this->portal2 = new Portal ();

	glm::vec3 portal1Position = glm::vec3 (0.5f, 0.0f, -(2.5f - epsilon));
	glm::vec3 portal2Position = glm::vec3 (0.5f, 0.0f, (2.5f - epsilon));

	this->portal1->generatePortalMesh ();
	this->portal2->generatePortalMesh ();

	this->portal1->toWorld = glm::translate (glm::mat4 (1.0f), portal1Position);

	//this->portal2->toWorld = glm::translate (glm::mat4 (1.0f), portal2Position) * glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f))
	//	* glm::rotate (glm::mat4 (1.0f), glm::radians (-45.0f), glm::vec3 (0.0f, 1.0f, 0.0f));

	this->portal2->toWorld = glm::translate (glm::mat4 (1.0f), portal2Position) * glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f));
}

const float Portal::PORTAL_SIZE = 0.5f;

const glm::vec3 Portal::vertices[] = {
	glm::vec3 (-Portal::PORTAL_SIZE / 2.0f, -Portal::PORTAL_SIZE / 2.0f, 0.0f),
	glm::vec3 (-Portal::PORTAL_SIZE / 2.0f, Portal::PORTAL_SIZE / 2.0f, 0.0f),
	glm::vec3 (Portal::PORTAL_SIZE / 2.0f, Portal::PORTAL_SIZE / 2.0f, 0.0f),
    glm::vec3 (Portal::PORTAL_SIZE / 2.0f, -Portal::PORTAL_SIZE / 2.0f, 0.0f)
};

const glm::vec3 Portal::normal = glm::vec3 (0.0f, 0.0f, 1.0f);

void Portal::generatePortalMesh ()
{
	this->model = new VAO ();

	int v0 = this->model->setXYZ (this->vertices[0])->setUV (0.0f, 0.0f)->pushVertex ();
	int v1 = this->model->setXYZ (this->vertices[1])->setUV (0.0f, 1.0f)->pushVertex ();
	int v2 = this->model->setXYZ (this->vertices[2])->setUV (1.0f, 1.0f)->pushVertex ();
	int v3 = this->model->setXYZ (this->vertices[3])->setUV (1.0f, 0.0f)->pushVertex ();

	this->model->pushIndex (v0)->pushIndex (v1)->pushIndex (v2);
	this->model->pushIndex (v2)->pushIndex (v3)->pushIndex (v0);
}

glm::vec3 Portal::getPosition ()
{
	return this->toWorld * glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec3 Portal::getNormal ()
{
	return glm::inverse (glm::transpose (this->toWorld)) * glm::vec4 (Portal::normal, 0.0f);
}

void Implementation::render ()
{

}

glm::mat4 Implementation::getNewCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal)
{
	static glm::mat4 rot180 = glm::rotate (glm::mat4 (1.0), glm::radians (180.0f), glm::vec3 (0.0, 1.0, 0.0));
	return currentViewMatrix * inPortal->toWorld * rot180 * glm::inverse (outPortal->toWorld);
}

void Implementation::tick ()
{
	player->tick ();
}

Implementation::~Implementation ()
{
}
