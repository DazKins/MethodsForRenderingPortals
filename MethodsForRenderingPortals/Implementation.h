#pragma once

#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Player.h"
#include "Level.h"

class Implementation
{
public:
	Implementation (Input* input, Window* window);
	~Implementation ();

	virtual void tick ();
	virtual void render ();

protected:
	Level * level;
	Input* input;
	Window* window;
	Camera* camera;
	Player* player;

	static float PORTAL_SIZE;
};
