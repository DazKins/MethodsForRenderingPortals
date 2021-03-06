#include "AutomaticCamera.h"
#include "Level.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

AutomaticCamera::AutomaticCamera (Window* window, Camera* camera, Implementation* implementation)
	: Player(0, window, camera, implementation)
{

	if (SCENE == 1 || SCENE == 2)
	{
		this->path = {
			{"mf", 60},
			{"mu", 5},
			{"mf", 60},
			{"md", 10},
			{"mf", 240},
		};
	}
	else
	{
		this->path = {
			{"mf", 60},
			{"mu", 5},
			{"mf", 60},
			{"md", 10},
			{"mf", 60},
			{"tl", 60},
			{"mf", 180},
		};
	}
}

AutomaticCamera::~AutomaticCamera ()
{
}

void AutomaticCamera::processInstruction (std::string instruction)
{
	if (instruction == "mf")
	{
		this->moveForward ();
	}
	else if (instruction == "ml")
	{
		this->moveLeft ();
	}
	else if (instruction == "mb")
	{
		this->moveBackward ();
	}
	else if (instruction == "mr")
	{
		this->moveRight ();
	}
	else if (instruction == "mu")
	{
		this->moveUp ();
	}
	else if (instruction == "md")
	{
		this->moveDown ();
	}
	else if (instruction == "md")
	{
		this->moveDown ();
	}
	else if (instruction == "tl")
	{
		this->rotate (glm::vec2 (0.0f, -5.0f));
	}
	else if (instruction == "tr")
	{
		this->rotate (glm::vec2 (0.0f, 5.0f));
	}
}

void AutomaticCamera::tick ()
{
	glm::vec3 lastFramePosition = this->position;

	if (currentStepCounter < this->path.size ())
	{
		std::tuple<std::string, unsigned int> currentInstruction = this->path[currentStepCounter];

		if (currentFrameCounter < std::get<1> (currentInstruction))
		{
			processInstruction (std::get<0> (currentInstruction));
			currentFrameCounter++;
		}
		else
		{
			currentFrameCounter = 0;
			currentStepCounter++;
		}
	}
	else
	{
		exit (0);
	}

	glm::vec3 thisFramePosition = this->position;

	handlePortalWalkthrough (this->implementation->getPortal1 (), this->implementation->getPortal2 (), lastFramePosition, thisFramePosition);
	handlePortalWalkthrough (this->implementation->getPortal2 (), this->implementation->getPortal1 (), lastFramePosition, thisFramePosition);

	camera->setPosition (this->position);
	camera->setRotation (this->rotation);
}