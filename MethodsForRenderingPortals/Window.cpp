#include "Window.h"

#include<iostream>

const char* Window::WINDOW_TITLE = "Methods For Rendering Portals";

Window::Window(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
}

bool Window::initializeGlad ()
{
	if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
}

GLFWwindow* Window::getWindowHandle () const
{
	return this->windowHandle;
}

bool Window::create ()
{
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

	this->windowHandle = glfwCreateWindow (this->width, this->height, this->WINDOW_TITLE, NULL, NULL);
	if (this->windowHandle == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate ();
		return false;
	}

	glfwMakeContextCurrent (this->windowHandle);

	if (!this->initializeGlad ()) return false;

	glViewport (0, 0, this->width, this->height);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(this->windowHandle);
}

void Window::setCursorPosition (glm::vec2 position)
{
	glfwSetCursorPos (this->windowHandle, position.x, position.y);
}

glm::vec2 Window::getMidPoint ()
{
	return glm::vec2 (this->width / 2.0, this->height / 2.0);
}

void Window::update()
{
	glfwSwapBuffers(this->windowHandle);
	glfwPollEvents();
}

Window::~Window() { }
