#include "Implementation.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"
#include "AutomaticCamera.h"
#include "Player.h"

Implementation::Implementation (Input* input, Window* window, int maxRecursionDepth, bool manualCamera)
{
	this->window = window;
	this->input = input;

	this->camera = new Camera (this->window);

	if (manualCamera)
		this->player = new AutomaticCamera (window, camera, this);
	else
		this->player = new Player (this->input, this->window, this->camera, this);

	this->level = new Level (5.0f, 1.0f, 5.0f);

	glm::mat4 projectionMatrix = glm::perspective (45.0f, this->window->getAspectRatio (), 0.001f, 1000.0f);
	for (Shader s : Shader::ALL_SHADERS)
	{
		s.bind ();
		Shader::setUniform ("projectionMatrix", projectionMatrix);
	}
	float epsilon = 0.05f;

	this->portal1 = new Portal ();
	this->portal2 = new Portal ();

	this->portal1->generatePortalMesh ();
	this->portal2->generatePortalMesh ();
	  
	glm::vec3 portal1Position = glm::vec3 (0.0f, 0.0f, -(2.5f - epsilon));
	this->portal1->setToWorld (glm::translate (glm::mat4 (1.0f), portal1Position));

	if (SCENE == 3)
	{
		glm::vec3 portal2Position = glm::vec3 (-2.0f, 0.0f, 2.0f);
		this->portal2->setToWorld (glm::translate (glm::mat4 (1.0f), portal2Position) * glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f))
			* glm::rotate (glm::mat4 (1.0f), glm::radians (-45.0f), glm::vec3 (0.0f, 1.0f, 0.0f)));
	}
	else
	{
		glm::vec3 portal2Position = glm::vec3 (0.0f, 0.0f, (2.5f - epsilon));
		this->portal2->setToWorld (glm::translate (glm::mat4 (1.0f), portal2Position) * glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f)));
	}

	this->maxRecursionDepth = maxRecursionDepth;

	portal1ViewOperators = (glm::mat4*) malloc (sizeof (glm::mat4) * this->maxRecursionDepth);
	portal2ViewOperators = (glm::mat4*) malloc (sizeof (glm::mat4) * this->maxRecursionDepth);

	portal1ViewOperators[0] = glm::mat4 (1.0f);
	portal2ViewOperators[0] = glm::mat4 (1.0f);

	for (int i = 1; i < maxRecursionDepth; i++)
	{
		portal1ViewOperators[i] = portal1ViewOperators[i - 1] * getNewCameraView (glm::mat4 (1.0f), portal1, portal2);
		portal2ViewOperators[i] = portal2ViewOperators[i - 1] * getNewCameraView (glm::mat4 (1.0f), portal2, portal1);
	}

	defaultProjection = glm::perspective (45.0f, this->window->getAspectRatio (), 0.001f, 1000.0f);
}

const float Portal::PORTAL_SIZE = 0.5f;

const glm::vec3 Portal::vertices[] = {
	glm::vec3 (-Portal::PORTAL_SIZE / 2.0f, -Portal::PORTAL_SIZE / 2.0f, 0.0f),
	glm::vec3 (-Portal::PORTAL_SIZE / 2.0f, Portal::PORTAL_SIZE / 2.0f, 0.0f),
	glm::vec3 (Portal::PORTAL_SIZE / 2.0f, Portal::PORTAL_SIZE / 2.0f, 0.0f),
    glm::vec3 (Portal::PORTAL_SIZE / 2.0f, -Portal::PORTAL_SIZE / 2.0f, 0.0f)
};

const glm::vec3 Portal::normal = glm::vec3 (0.0f, 0.0f, 1.0f);

void Portal::setToWorld (glm::mat4 toWorld)
{
	this->toWorld = toWorld;
	this->invToWorld = glm::inverse (toWorld);

	for (int i = 0; i < 4; i++)
		this->worldVertices[i] = toWorld * glm::vec4(Portal::vertices[i], 1.0f);
}

glm::mat4 Portal::getToWorld ()
{
	return this->toWorld;
}

glm::mat4 Portal::getInvToWorld ()
{
	return this->invToWorld;
}

glm::vec3 * Portal::getWorldVertices ()
{
	return worldVertices;
}

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

Portal *Implementation::getPortal1 () const
{
	return portal1;
}

Portal *Implementation::getPortal2 () const
{
	return portal2;
}


glm::mat4 Implementation::getNewCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal)
{
	static glm::mat4 rot180 = glm::rotate (glm::mat4 (1.0), glm::radians (180.0f), glm::vec3 (0.0, 1.0, 0.0));
	return currentViewMatrix * inPortal->getToWorld () * rot180 * outPortal->getInvToWorld ();
}

glm::mat4 Implementation::getOldCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal)
{
	static glm::mat4 rot180 = glm::rotate (glm::mat4 (1.0), glm::radians (180.0f), glm::vec3 (0.0, 1.0, 0.0));
	return outPortal->getToWorld() * rot180 * inPortal->getInvToWorld() * glm::inverse (currentViewMatrix);
}

void Implementation::tick ()
{
	player->tick ();
	level->tick ();
}

Implementation::~Implementation ()
{
}
