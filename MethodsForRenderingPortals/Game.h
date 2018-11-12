#pragma once

#include "Window.h"
#include "Input.h"

class Game
{
public:
	Game (Input *input, Window* window);
	~Game ();

	void render ();
	void tick ();
};

