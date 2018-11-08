#pragma once

#include <glm/glm.hpp>

#include "Input.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec2 rotation;
	Input *input;
	Window *window;

public:
	Camera (Input *input, Window *window);
	~Camera ();

	glm::mat4 getViewMatrix ();

	void tick ();
};

