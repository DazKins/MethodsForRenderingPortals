#include "Player.h"

Player::Player (Input* input, Window* window, Camera* camera)
{
	this->input = input;
	this->window = window;
	this->camera = camera;
}

Player::~Player () { }

float rotationSpeed = 0.002f;
//float movementSpeed = 0.005f
float movementSpeed = 0.025f;

void Player::tick ()
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

	window->setCursorPosition (window->getMidPoint ());

	camera->setPosition (this->position);
	camera->setRotation (this->rotation);
}