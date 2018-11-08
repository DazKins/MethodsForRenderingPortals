#include "Input.h"

#include <GLFW/glfw3.h>

Input::Input(const Window* window)
{
	this->window = window;
}

bool Input::isKeyDown(unsigned int key)
{
	return glfwGetKey(this->window->getWindowHandle(), key);
}

glm::vec2 Input::getMousePosition ()
{
	double xpos = 0.0;
	double ypos = 0.0;
	glfwGetCursorPos (this->window->getWindowHandle(), &xpos, &ypos);
	return glm::vec2 (xpos, ypos);
}

Input::~Input()
{
}