#pragma once

#include "VAO.h"
#include "Texture.h"

extern unsigned int SCENE;

class Level
{
public:
	Level (float xSize, float ySize, float zSize);
	~Level ();

	void render ();
	void tick ();

private:
	VAO* wallVAO;
	VAO* floorVAO;

	Texture* wallTexture;
	Texture* floorTexture;

	VAO* bunny;
};

