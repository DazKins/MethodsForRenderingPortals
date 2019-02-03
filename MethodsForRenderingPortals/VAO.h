#pragma once

#include<vector>
#include <glm/glm.hpp>

class VAO
{
private:
	unsigned int vaoId;

	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	unsigned int vertexCount;
	std::vector<float> vertexData;

	unsigned int indexCount;
	std::vector<unsigned int> indexData;

	bool compiled = false;

public:
	VAO ();
	~VAO ();

	VAO* setX (float x);
	VAO* setY (float y);
	VAO* setZ (float z);
	VAO* setXYZ (float x, float y, float z);
	VAO* setXYZ (glm::vec3 xyz);

	VAO* setNormalX (float x);
	VAO* setNormalY (float y);
	VAO* setNormalZ (float z);
	VAO* setNormalXYZ (float x, float y, float z);
	VAO* setNormalXYZ (glm::vec3 xyz);

	VAO* setU (float u);
	VAO* setV (float v);
	VAO* setUV (float u, float v);

	int pushVertex ();

	VAO* pushIndex (int index);

	bool isCompiled ();

	void compile ();

	void render ();
};

