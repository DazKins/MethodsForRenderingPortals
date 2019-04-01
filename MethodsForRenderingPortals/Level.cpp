#include "Level.h"

#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "Shader.h"
#include "ModelLoader.h"

Level::Level (float xSize, float ySize, float zSize)
{
	float x2 = xSize / 2.0f;
	float y2 = ySize / 2.0f;
	float z2 = zSize / 2.0f;

	wallVAO = new VAO ();
	floorVAO = new VAO ();

	int frontTopRight = wallVAO->setXYZ (x2, y2, -z2)->setUV (xSize, 0.0f)->pushVertex ();
	int frontBottomRight = wallVAO->setXYZ (x2, -y2, -z2)->setUV (xSize, ySize)->pushVertex ();
	int frontBottomLeft = wallVAO->setXYZ (-x2, -y2, -z2)->setUV (0.0, ySize)->pushVertex ();
	int frontTopLeft = wallVAO->setXYZ (-x2, y2, -z2)->setUV (0.0, 0.0f)->pushVertex ();

	wallVAO->pushIndex (frontTopRight)->pushIndex (frontBottomRight)->pushIndex (frontTopLeft);
	wallVAO->pushIndex (frontBottomRight)->pushIndex (frontBottomLeft)->pushIndex (frontTopLeft);

	int backTopRight = wallVAO->setXYZ (x2, y2, z2)->setUV (xSize, 0.0f)->pushVertex ();
	int backBottomRight = wallVAO->setXYZ (x2, -y2, z2)->setUV (xSize, ySize)->pushVertex ();
	int backBottomLeft = wallVAO->setXYZ (-x2, -y2, z2)->setUV (0.0, ySize)->pushVertex ();
	int backTopLeft = wallVAO->setXYZ (-x2, y2, z2)->setUV (0.0, 0.0f)->pushVertex ();

	wallVAO->pushIndex (backTopRight)->pushIndex (backBottomRight)->pushIndex (backTopLeft);
	wallVAO->pushIndex (backBottomRight)->pushIndex (backBottomLeft)->pushIndex (backTopLeft);

	int leftTopRight = wallVAO->setXYZ (x2, y2, z2)->setUV (zSize, 0.0f)->pushVertex ();
	int leftBottomRight = wallVAO->setXYZ (x2, -y2, z2)->setUV (zSize, ySize)->pushVertex ();
	int leftBottomLeft = wallVAO->setXYZ (x2, -y2, -z2)->setUV (0.0, ySize)->pushVertex ();
	int leftTopLeft = wallVAO->setXYZ (x2, y2, -z2)->setUV (0.0, 0.0f)->pushVertex ();

	wallVAO->pushIndex (leftTopRight)->pushIndex (leftBottomRight)->pushIndex (leftTopLeft);
	wallVAO->pushIndex (leftBottomRight)->pushIndex (leftBottomLeft)->pushIndex (leftTopLeft);

	int rightTopRight = wallVAO->setXYZ (-x2, y2, z2)->setUV (zSize, 0.0f)->pushVertex ();
	int rightBottomRight = wallVAO->setXYZ (-x2, -y2, z2)->setUV (zSize, ySize)->pushVertex ();
	int rightBottomLeft = wallVAO->setXYZ (-x2, -y2, -z2)->setUV (0.0, ySize)->pushVertex ();
	int rightTopLeft = wallVAO->setXYZ (-x2, y2, -z2)->setUV (0.0, 0.0f)->pushVertex ();

	wallVAO->pushIndex (rightTopRight)->pushIndex (rightBottomRight)->pushIndex (rightTopLeft);
	wallVAO->pushIndex (rightBottomRight)->pushIndex (rightBottomLeft)->pushIndex (rightTopLeft);

	int floorTopRight = floorVAO->setXYZ (x2, -y2, z2)->setUV (zSize, 0.0f)->pushVertex ();
	int floorBottomRight = floorVAO->setXYZ (-x2, -y2, z2)->setUV (zSize, xSize)->pushVertex ();
	int floorBottomLeft = floorVAO->setXYZ (-x2, -y2, -z2)->setUV (0.0, xSize)->pushVertex ();
	int floorTopLeft = floorVAO->setXYZ (x2, -y2, -z2)->setUV (0.0, 0.0f)->pushVertex ();

	floorVAO->pushIndex (floorTopRight)->pushIndex (floorBottomRight)->pushIndex (floorTopLeft);
	floorVAO->pushIndex (floorBottomRight)->pushIndex (floorBottomLeft)->pushIndex (floorTopLeft);

	this->wallTexture = new Texture ("textures/wall.jpg");
	this->floorTexture = new Texture ("textures/floor.jpg");

	bunny = loadObj ("models/bunny");
}

Level::~Level () { }

float t = 0.0f;

void Level::render ()
{
	this->wallTexture->bind ();
	this->wallVAO->render ();

	this->floorTexture->bind ();
	this->floorVAO->render ();

	Shader::updateAllModelMatrices (glm::translate (glm::mat4(1.0f), glm::vec3 (-sin(t), 0.0f, 0.0f)));
	Shader::DEFAULT->bind ();
	this->bunny->render ();
	Shader::updateAllModelMatrices (glm::mat4 (1.0f));
}

void Level::tick ()
{
	t += 0.01f;
}