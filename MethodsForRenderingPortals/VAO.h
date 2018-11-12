#pragma once

#include<vector>

class VAO
{
private:
	unsigned int vaoId;

	float x;
	float y;
	float z;
	float u;
	float v;

	unsigned int vertexCount;
	std::vector<float> vertexData;

	unsigned int indexCount;
	std::vector<unsigned int> indexData;

public:
	VAO ();
	~VAO ();

	VAO* setX (float x);
	VAO* setY (float y);
	VAO* setZ (float z);
	VAO* setXYZ (float x, float y, float z);

	VAO* setU (float u);
	VAO* setV (float v);
	VAO* setUV (float u, float v);

	int pushVertex ();

	VAO* pushIndex (int index);

	void compile ();

	void render ();
};

