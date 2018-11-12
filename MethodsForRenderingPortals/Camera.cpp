#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

Camera::Camera (Input* input, Window* window)
{
	this->input = input;
	this->window = window;
}

float rotationSpeed = 0.002f;
float movementSpeed = 0.005f;

void Camera::tick ()
{
	glm::vec2 midPoint = window->getMidPoint ();
	glm::vec2 mousePosition = input->getMousePosition ();

	glm::vec2 d = mousePosition - midPoint;

	this->rotation.x -= d.y * rotationSpeed;
	this->rotation.y -= d.x * rotationSpeed;

	float sinY = sin (this->rotation.y);
	float cosY = cos (this->rotation.y);

	if (this->input->isKeyDown (GLFW_KEY_W))
		this->position += glm::vec3 (-movementSpeed * sinY, 0, -movementSpeed * cosY);
	if (this->input->isKeyDown (GLFW_KEY_A))
		this->position += glm::vec3 (-movementSpeed * cosY, 0, movementSpeed * sinY);
	if (this->input->isKeyDown (GLFW_KEY_S))
		this->position += glm::vec3 (movementSpeed * sinY, 0, movementSpeed * cosY);
	if (this->input->isKeyDown (GLFW_KEY_D))
		this->position += glm::vec3 (movementSpeed * cosY, 0, -movementSpeed * sinY);

	if (this->input->isKeyDown (GLFW_KEY_SPACE))
		this->position += glm::vec3 (0.0f, movementSpeed, 0.0f);
	if (this->input->isKeyDown (GLFW_KEY_LEFT_CONTROL))
		this->position += glm::vec3 (0.0f, -movementSpeed, 0.0f);

	window->setCursorPosition(window->getMidPoint());
}

glm::mat4 Camera::getViewMatrix ()
{
	glm::mat4 rotateX = glm::rotate (glm::mat4 (1.0), -this->rotation.x, glm::vec3 (1.0, 0.0, 0.0));
	glm::mat4 rotateY = glm::rotate (rotateX, -this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
	return glm::translate (rotateY, -this->position);
}

Camera::~Camera ()
{
}