#pragma once

#include "Window.h"

class Input
{
public:
	Input(const Window* window);
	~Input();
	bool isKeyDown(unsigned int key);

private:
	const Window* window;
};

