#include "Implementation.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Shader.h"

float Implementation::PORTAL_SIZE = 0.5f;

Implementation::Implementation (Input* input, Window* window)
{
	this->window = window;
	this->input = input;

	this->camera = new Camera (this->window);
	this->player = new Player (this->input, this->window, this->camera);

	this->level = new Level(5.0f, 1.0f, 5.0f);

	glm::mat4 projectionMatrix = glm::perspective (45.0f, this->window->getAspectRatio (), 0.1f, 100.0f);
	Shader::updateAllProjectionMatrices (projectionMatrix);
}

void Implementation::render ()
{

}

void Implementation::tick ()
{
	player->tick ();
}

Implementation::~Implementation ()
{
}