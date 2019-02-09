#include "Player.h"

#include <math.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Implementation.h"

Player::Player (Input* input, Window* window, Camera* camera, Implementation* implementation)
{
	this->input = input;
	this->window = window;
	this->camera = camera;
	this->implementation = implementation;
}

Player::~Player () { }

float rotationSpeed = 0.002f;
float movementSpeed = 0.025f;

void Player::tick ()
{
	glm::vec3 lastFramePosition = this->position;

	glm::vec2 midPoint = window->getMidPoint ();
	glm::vec2 mousePosition = input->getMousePosition ();

	glm::vec2 dp = mousePosition - midPoint;

	this->rotation.x -= dp.y * rotationSpeed;
	this->rotation.y -= dp.x * rotationSpeed;

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

	window->setCursorPosition (window->getMidPoint ());

	glm::vec3 thisFramePosition = this->position;

	handlePortalWalkthrough (this->implementation->getPortal1 (), this->implementation->getPortal2 (), lastFramePosition, thisFramePosition);
	handlePortalWalkthrough (this->implementation->getPortal2 (), this->implementation->getPortal1 (), lastFramePosition, thisFramePosition);

	camera->setPosition (this->position);
	camera->setRotation (this->rotation);
}

void Player::handlePortalWalkthrough (Portal *inPortal, Portal *outPortal, glm::vec3 lastFramePosition, glm::vec3 thisFramePosition)
{
	glm::vec3 d = thisFramePosition - lastFramePosition;
	glm::vec3 mu = lastFramePosition;

	float lambda = glm::dot ((inPortal->getPosition () - mu), inPortal->getNormal ()) / glm::dot (d, inPortal->getNormal ());

	if (lambda <= 1 && lambda >= 0)
	{
		glm::vec3 collisionPos = lambda * d + mu;

		glm::vec3 inPortalVertices[4];

		for (int i = 0; i < 4; i++)
			inPortalVertices[i] = inPortal->toWorld * glm::vec4 (Portal::vertices[i], 1.0f);

		float A0 = 0.5f * glm::length (glm::cross ((inPortalVertices[0] - collisionPos), (inPortalVertices[1] - collisionPos)));
		float A1 = 0.5f * glm::length (glm::cross ((inPortalVertices[1] - collisionPos), (inPortalVertices[2] - collisionPos)));
		float A2 = 0.5f * glm::length (glm::cross ((inPortalVertices[2] - collisionPos), (inPortalVertices[3] - collisionPos)));
		float A3 = 0.5f * glm::length (glm::cross ((inPortalVertices[3] - collisionPos), (inPortalVertices[0] - collisionPos)));
		float TA = A0 + A1 + A2 + A3;

		if (TA <= Portal::PORTAL_SIZE * Portal::PORTAL_SIZE)
		{
			static glm::mat4 rot180 = glm::rotate (glm::mat4 (1.0f), glm::radians (180.0f), glm::vec3 (0.0f, 1.0f, 0.0f));

			glm::vec3 newPos = outPortal->toWorld * rot180 * glm::inverse (inPortal->toWorld) * glm::vec4 (thisFramePosition, 1.0f);

			position = newPos;

			glm::vec3 rotationAxis = glm::cross (inPortal->getNormal (), outPortal->getNormal ());
			float rotation = glm::dot (inPortal->getNormal (), outPortal->getNormal ());

			glm::mat4 piToPo = outPortal->toWorld * rot180 * glm::inverse (inPortal->toWorld);

			this->rotation.x -= atan2f (piToPo[2][1], piToPo[2][2]);
			this->rotation.y -= atan2f (-piToPo[2][0], sqrt (piToPo[2][1] * piToPo[2][1] + piToPo[2][2] * piToPo[2][2]));
			// let's try not to rotate around z for now...
		}
	}
}