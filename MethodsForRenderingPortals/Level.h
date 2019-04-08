#pragma once

#include "VAO.h"
#include "Texture.h"

constexpr auto SCENE = 3;

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

