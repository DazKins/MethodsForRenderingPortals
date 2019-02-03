#pragma once

#include <glm/glm.hpp>

#include "Input.h"
#include "Camera.h"

class Player
{
private:
	glm::vec3 position;
	glm::vec3 rotation;

	Input* input;
	Window* window;
	Camera* camera;

public:
	Player (Input* input, Window* window, Camera* camera);
	~Player ();

	void tick ();
};

