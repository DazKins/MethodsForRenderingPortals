#pragma once

#include <glm/glm.hpp>

#include "Window.h"

class Input
{
public:
	Input(const Window* window);
	~Input();
	bool isKeyDown(unsigned int key);
	glm::vec2 getMousePosition();

private:
	const Window* window;
};

