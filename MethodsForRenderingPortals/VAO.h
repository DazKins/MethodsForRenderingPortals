#pragma once

#include<vector>

class VAO
{
private:
	unsigned int vaoId;

	float x;
	float y;
	float z;

	unsigned int vertexCount;
	std::vector<float> vertexPositionData;

	unsigned int indexCount;
	std::vector<unsigned int> indexData;

public:
	VAO ();
	~VAO ();

	VAO* setX (float x);
	VAO* setY (float y);
	VAO* setZ (float z);
	VAO* setXYZ (float x, float y, float z);

	int pushVertex ();

	VAO* pushIndex (int index);

	void compile ();

	void render ();
};

