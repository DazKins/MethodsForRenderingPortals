#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

Camera::Camera (Window* window)
{
	this->window = window;
}

void Camera::tick ()
{
}

glm::mat4 Camera::getViewMatrix ()
{
	glm::mat4 rotateX = glm::rotate (glm::mat4 (1.0), -this->rotation.x, glm::vec3 (1.0, 0.0, 0.0));
	glm::mat4 rotateY = glm::rotate (rotateX, -this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
	return glm::translate (rotateY, -this->position);
}

glm::mat4 Camera::getTranslationMatrix ()
{
	return glm::translate (glm::mat4 (1.0), -this->position);
}

Camera::~Camera ()
{
}

void Camera::setPosition (glm::vec3 position)
{
	this->position = position;
}

void Camera::setRotation (glm::vec2 rotation)
{
	this->rotation = rotation;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}
