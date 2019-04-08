#pragma once
#include "Player.h"

#include <vector>
#include <string>
#include <tuple>

class Implementation;

class AutomaticCamera : public Player
{
public:
	AutomaticCamera (Window* window, Camera* camera, Implementation* implementation);
	~AutomaticCamera ();

	void tick ();

private:
	std::vector<std::tuple<std::string, unsigned int>> path;

	void processInstruction (std::string instruction);

	unsigned int currentFrameCounter;
	unsigned int currentStepCounter;
};

