#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	unsigned int width;
	unsigned int height;
	GLFWwindow* windowHandle;

	bool initializeGlad ();

	static const char* WINDOW_TITLE;

public:
	Window(unsigned int width, unsigned int height);
	bool create ();
	bool shouldClose();
	void update();
	GLFWwindow* getWindowHandle () const;
	~Window ();
};

