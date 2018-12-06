#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

float PORTAL_SIZE = 0.5f;

Game::Game (Input *input, Window* window)
{
	Shader::DEFAULT->bind ();

	glm::mat4 projectionMatrix = glm::perspective (45.0f, window->getAspectRatio(), 0.1f, 100.0f);
	Shader::updateAllProjectionMatrices (projectionMatrix);

	this->camera = new Camera (window);

	this->level = new Level (5.0f, 1.0f, 5.0f);

	this->player = new Player (input, window, camera);

	float epsilon = 0.5f;

	this->portal1Position = glm::vec3(0.0f, 0.0f, -(2.5f - epsilon));
	//this->portal1Normal = portal1Normal;
	this->portal2Position = glm::vec3(0.0f, 0.0f, 2.5f - epsilon);
	//this->portal2Normal = portal2Normal;

	this->portal1 = new VAO();
	this->portal2 = new VAO();

	int v0 = this->portal1->setXYZ(glm::vec3(-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex();
	int v1 = this->portal1->setXYZ(glm::vec3(-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex();
	int v2 = this->portal1->setXYZ(glm::vec3(PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex();
	int v3 = this->portal1->setXYZ(glm::vec3(PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal1Position)->pushVertex();

	this->portal1->pushIndex(v0)->pushIndex(v1)->pushIndex(v2);
	this->portal1->pushIndex(v2)->pushIndex(v3)->pushIndex(v0);

	this->portal1->compile();

	v0 = this->portal2->setXYZ(glm::vec3(-PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v1 = this->portal2->setXYZ(glm::vec3(-PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v2 = this->portal2->setXYZ(glm::vec3(PORTAL_SIZE / 2.0f, PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();
	v3 = this->portal2->setXYZ(glm::vec3(PORTAL_SIZE / 2.0f, -PORTAL_SIZE / 2.0f, 0.0f) + portal2Position)->pushVertex();

	this->portal2->pushIndex(v0)->pushIndex(v1)->pushIndex(v2);
	this->portal2->pushIndex(v2)->pushIndex(v3)->pushIndex(v0);

	this->portal2->compile();

	this->input = input;
}

Game::~Game () { }

void Game::render ()
{
	renderFromPerspective(this->camera);
}

void Game::tick ()
{
	player->tick ();
}

void Game::renderFromPerspective(Camera* cam)
{
	glm::mat4 viewMatrix = cam->getViewMatrix();
	Shader::updateAllViewMatrices (viewMatrix);

	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	Shader::DEFAULT->bind ();
	this->portal1->render();

	glStencilFunc (GL_ALWAYS, 2, 0xFF);
	Shader::DEFAULT->bind ();
	this->portal2->render ();

	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	Shader::DEFAULT->bind ();
	level->render();

	// PORTAL 1

	glm::vec3 position = cam->getPosition();
	cam->setPosition (position - portal1Position + portal2Position);
	viewMatrix = cam->getViewMatrix();
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc(GL_EQUAL, 1, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal2Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", glm::vec3 (0.0f, 0.0f, -1.0f));
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render();

	// PORTAL 2

	cam->setPosition(position - portal2Position + portal1Position);
	viewMatrix = cam->getViewMatrix();
	Shader::updateAllViewMatrices (viewMatrix);

	glStencilFunc(GL_EQUAL, 2, 0xFF);

	Shader::PORTAL_CLIP->setUniform ("portalPosition", portal1Position);
	Shader::PORTAL_CLIP->setUniform ("portalNormal", glm::vec3 (0.0f, 0.0f, 1.0f));
	Shader::PORTAL_CLIP->bind ();

	glClear (GL_DEPTH_BUFFER_BIT);
	level->render();

	Shader::DEFAULT->bind ();
	glDisable(GL_STENCIL_TEST);
	cam->setPosition(position);
}
