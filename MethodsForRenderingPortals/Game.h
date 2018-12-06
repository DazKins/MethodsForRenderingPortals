#pragma once

#include "Window.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"
#include "Level.h"
#include "Player.h"

class Game
{
public:
	Game (Input* input, Window* window);
	~Game ();

	void render ();
	void tick ();

	void renderFromPerspective (Camera* cam);

private:
	Level * level;
	Camera* camera;
	Player* player;
	Input* input;

	glm::vec3 portal1Position;
	glm::vec3 portal1Normal;

	glm::vec3 portal2Position;
	glm::vec3 portal2Normal;

	VAO* portal1;
	VAO* portal2;
};

