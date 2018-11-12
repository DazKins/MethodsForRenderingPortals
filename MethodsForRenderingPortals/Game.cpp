#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Level.h"
#include "Player.h"

Level* level;
Camera* camera;
Player* player;

Game::Game (Input *input, Window* window)
{
	Shader::DEFAULT_SHADER->bind ();

	glm::mat4 projectionMatrix = glm::perspective (45.0f, window->getAspectRatio(), 0.1f, 100.0f);
	Shader::DEFAULT_SHADER->setUniform ("projectionMatrix", projectionMatrix);

	camera = new Camera (input, window);

	level = new Level (5.0f, 1.0f, 5.0f);

	player = new Player (input, window, camera);
}

Game::~Game () { }

void Game::render ()
{
	glm::mat4 viewMatrix = camera->getViewMatrix ();
	Shader::DEFAULT_SHADER->setUniform ("viewMatrix", viewMatrix);

	level->render ();
}

void Game::tick ()
{
	player->tick ();
}