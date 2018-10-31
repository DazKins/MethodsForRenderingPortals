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

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

	defaultShader = new Shader(vertexShaderSource, fragmentShaderSource);

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