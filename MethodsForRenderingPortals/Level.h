#pragma once

#include "VAO.h"
#include "Texture.h"

class Level
{
public:
	Level (float xSize, float ySize, float zSize);
	~Level ();

	void render ();

private:
	VAO* wallVAO;
	VAO* floorVAO;

	Texture* wallTexture;
	Texture* floorTexture;

	VAO* bunny;
};

