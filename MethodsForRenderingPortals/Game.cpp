#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VAO.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"

VAO* vao;
Camera* camera;
Texture* texture;
Shader* defaultShader;

Game::Game (Input *input, Window* window) 
{
	defaultShader = new Shader ("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	defaultShader->bind ();

	glm::mat4 projectionMatrix = glm::perspective (45.0f, 1.0f, 0.1f, 100.0f);

	defaultShader->setUniform ("projectionMatrix", projectionMatrix);

	vao = new VAO ();

	int topRight = vao->setXYZ (0.5f, 0.5f, -0.5f)->setUV (1.0, 0.0f)->pushVertex ();
	int bottomRight = vao->setXYZ (0.5f, -0.5f, -0.5f)->setUV (1.0, 1.0f)->pushVertex ();
	int bottomLeft = vao->setXYZ (-0.5f, -0.5f, -0.5f)->setUV (0.0, 1.0f)->pushVertex ();
	int topLeft = vao->setXYZ (-0.5f, 0.5f, -0.5f)->setUV (0.0, 0.0f)->pushVertex ();

	vao->pushIndex (topRight)->pushIndex (bottomRight)->pushIndex (topLeft);
	vao->pushIndex (bottomRight)->pushIndex (bottomLeft)->pushIndex (topLeft);

	vao->compile ();

	camera = new Camera (input, window);

	texture = new Texture ("textures/wall.jpg");
}

Game::~Game () { }

float t = 0.0f;

void Game::render ()
{
	glm::mat4 viewMatrix = camera->getViewMatrix ();
	defaultShader->setUniform ("viewMatrix", viewMatrix);

	glm::mat4 modelMatrix = glm::rotate (glm::mat4 (1.0), t += 0.001f, glm::vec3 (0.0, 0.0, 1.0));
	defaultShader->setUniform ("modelMatrix", modelMatrix);

	texture->bind ();
	vao->render ();
}

void Game::tick ()
{
	camera->tick ();
}