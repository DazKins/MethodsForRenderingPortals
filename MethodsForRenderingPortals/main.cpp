#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"
#include "Input.h"
#include "ImplementationStencilBuffer.h"
#include "ImplementationFramebufferObjects.h"
#include "Shader.h"

const int INITIAL_WINDOW_WIDTH = 1280;
const int INITIAL_WINDOW_HEIGHT = 720;

bool running = false;

Window* window;
Input* input;
Implementation* implementation;

void tick ()
{
	if (input->isKeyDown (GLFW_KEY_ESCAPE))
		running = false;

	implementation->tick ();
}

void render ()
{
	glm::vec4 clearColor = glm::vec4 (0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor (clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	Shader::PORTAL_STENCIL_BUFFER->setUniform ("clearColor", clearColor);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	implementation->render ();

	window->update ();
}

void init ()
{
	glfwInit ();

	window = new Window (INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	window->create ();

	glEnable (GL_DEPTH_TEST);

	input = new Input (window);

	Shader::initShaders ();

	//implementation = static_cast<Implementation*> (new ImplementationStencilBuffer (input, window));
	implementation = static_cast<Implementation*> (new ImplementationFramebufferObjects (input, window));

	window->hideCursor ();
}

void mainLoop ()
{
	while (running)
	{
		tick ();
		render ();

		if (window->shouldClose ())
			running = false;
	}
}

int main ()
{
	init ();

	running = true;

	mainLoop ();

	glfwTerminate ();

	return 0;
}