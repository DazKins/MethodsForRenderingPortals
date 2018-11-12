#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"
#include "Input.h"
#include "Game.h"
#include "Shader.h"

const int INITIAL_WINDOW_WIDTH = 1280;
const int INITIAL_WINDOW_HEIGHT = 720;

bool running = false;

Window* window;
Input* input;
Game* game;

void tick()
{
	if (input->isKeyDown(GLFW_KEY_ESCAPE))
		running = false;

	game->tick ();
}

void render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	game->render ();

	window->update();
}

void init()
{
	glfwInit();

	window = new Window(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	window->create();

	glEnable (GL_DEPTH_TEST);

	input = new Input(window);

	Shader::initShaders ();

	game = new Game (input, window);

	window->hideCursor ();
}

void mainLoop()
{
	while (running)
	{
		tick();
		render();

		if (window->shouldClose())
			running = false;
	}
}

int main ()
{
	init();

	running = true;

	mainLoop();

	glfwTerminate();

	return 0;
}