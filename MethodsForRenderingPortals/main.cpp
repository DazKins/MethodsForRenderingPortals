#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "VAO.h"
#include "Input.h"

const int INITIAL_WINDOW_WIDTH = 1280;
const int INITIAL_WINDOW_HEIGHT = 720;

Window* window;
Shader* defaultShader;
Input* input;
VAO* vao;

bool running = false;

void tick()
{
	if (input->isKeyDown(GLFW_KEY_ESCAPE))
		running = false;
}

void render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	vao->render();

	window->update();
}

void init()
{
	glfwInit();

	window = new Window(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	window->create();

	defaultShader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	defaultShader->bind();

	vao = new VAO();

	int topRight = vao->setXYZ(0.5f, 0.5f, 0.0f)->pushVertex();
	int bottomRight = vao->setXYZ(0.5f, -0.5f, 0.0f)->pushVertex();
	int bottomLeft = vao->setXYZ(-0.5f, -0.5f, 0.0f)->pushVertex();
	int topLeft = vao->setXYZ(-0.5f, 0.5f, 0.0f)->pushVertex();

	vao->pushIndex(topRight)->pushIndex(bottomRight)->pushIndex(topLeft);
	vao->pushIndex(bottomRight)->pushIndex(bottomLeft)->pushIndex(topLeft);

	vao->compile();

	input = new Input(window);
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