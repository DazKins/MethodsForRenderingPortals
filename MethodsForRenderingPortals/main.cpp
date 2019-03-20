#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

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
	std::string option;
	do
	{
		std::cout << "Enter f for FBO implementation, s for stencil buffer" << std::endl;
		std::cin >> option;
	} while (option[0] != 'f' && option[0] != 'F' && option[0] != 's' && option[0] != 'S');

	glfwInit ();

	window = new Window (INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	window->create ();

	glEnable (GL_DEPTH_TEST);

	input = new Input (window);

	Shader::initShaders ();

	if (option[0] == 'f' || option[0] == 'F')
		implementation = static_cast<Implementation*> (new ImplementationFramebufferObjects (input, window));
	else if (option[0] == 's' || option[0] == 'S')
		implementation = static_cast<Implementation*> (new ImplementationStencilBuffer (input, window));

	window->hideCursor ();
}

void mainLoop ()
{
	float delta = 0.0f;
	float nsPerTick = 1000000000.0f / 60.0f;
	auto lastTime = std::chrono::high_resolution_clock::now ();

	int secondsPerDebugOutput = 1;
	auto lastDebugOutput = std::chrono::system_clock::now ().time_since_epoch ();

	long totalFrameTime = 0;
	int frameCount = 0;

	while (running)
	{
		auto now = std::chrono::high_resolution_clock::now ();
		delta += std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count() / nsPerTick;
		lastTime = now;

		while (delta >= 1.0f)
		{
			tick ();
			delta -= 1.0f;
		}

		auto before = std::chrono::high_resolution_clock::now ();
		render ();
		glFinish ();
		auto after = std::chrono::high_resolution_clock::now ();

		long frameTimeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count ();
		frameCount++;
		totalFrameTime += frameTimeNs;

		if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now ().time_since_epoch () - lastDebugOutput)).count() >= secondsPerDebugOutput * 1000)
		{
			lastDebugOutput = std::chrono::system_clock::now ().time_since_epoch ();
			float avgFrameTime = totalFrameTime / frameCount;
			totalFrameTime = 0;
			frameCount = 0;
			std::cout << "Avg. frame time: " << avgFrameTime / 1000000 << "ms (" << avgFrameTime << "ns)" << std::endl;
		}

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