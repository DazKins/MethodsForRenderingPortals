#pragma once

#include <glm/glm.hpp>

#include "Input.h"
#include "Camera.h"
#include "Implementation.h"

class Implementation;
struct Portal;

class Player
{
private:
	glm::vec3 position;
	glm::vec3 rotation;

	Input* input;
	Window* window;
	Camera* camera;
	Implementation* implementation;

	void handlePortalWalkthrough (Portal *inPortal, Portal *outPortal, glm::vec3 lastFramePosition, glm::vec3 thisFramePosition);

public:
	Player (Input* input, Window* window, Camera* camera, Implementation* implementation);
	~Player ();

	void moveForward ();
	void moveBackward ();
	void moveLeft ();
	void moveRight ();
	void moveUp ();
	void moveDown ();

	void rotate (glm::vec2 rot);

	virtual void tick ();

	friend class AutomaticCamera;
};

