#pragma once

#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Player.h"

class Implementation
{
public:
	Implementation (Input* input, Window* window);
	~Implementation ();

	virtual void tick ();
	virtual void render ();

protected:
	Input* input;
	Window* window;
	Camera* camera;
	Player* player;
};

