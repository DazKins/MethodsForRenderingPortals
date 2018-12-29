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

	glfwSwapInterval (1);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(this->windowHandle);
}

void Window::setCursorPosition (glm::vec2 position)
{
	glfwSetCursorPos (this->windowHandle, position.x, position.y);
}

void Window::hideCursor ()
{
	glfwSetInputMode (this->windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::vec2 Window::getMidPoint ()
{
	return glm::vec2 (this->width / 2.0, this->height / 2.0);
}

float Window::getAspectRatio ()
{
	return (float) this->width / this->height;
}

unsigned int Window::getWidth ()
{
	return this->width;
}

unsigned int Window::getHeight ()
{
	return this->height;
}

void Window::update()
{
	glfwSwapBuffers(this->windowHandle);
	glfwPollEvents();
}

Window::~Window() { }
