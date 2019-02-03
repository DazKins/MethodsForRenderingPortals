#pragma once

#include <glm/glm.hpp>

#include "Input.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	Window *window;

public:
	Camera (Window *window);
	~Camera ();

	void setPosition (glm::vec3 position);
	void setRotation (glm::vec3 rotation);

	glm::vec3 getPosition ();
	glm::vec3 getRotation ();

	glm::mat4 getViewMatrix ();
	glm::mat4 getTranslationMatrix ();

	void tick ();
};

