#include "Input.h"

Input::Input(const Window* window)
{
	this->window = window;
}

bool Input::isKeyDown(unsigned int key)
{
	return glfwGetKey(this->window->getWindowHandle(), key);
}

Input::~Input()
{
}