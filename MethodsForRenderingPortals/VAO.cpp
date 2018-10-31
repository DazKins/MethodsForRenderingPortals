#include "VAO.h"

#include <glad/glad.h>
#include <iostream>

VAO::VAO()
{
	glGenVertexArrays(1, &this->vaoId);

	this->vertexCount = 0;
}

VAO::~VAO()
{

}

VAO* VAO::setX(float x)
{
	this->x = x;
	return this;
}

VAO* VAO::setY(float y)
{
	this->y = y;
	return this;
}

VAO* VAO::setZ(float z)
{
	this->z = z;
	return this;
}

VAO* VAO::setXYZ(float x, float y, float z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
	return this;
}

int VAO::pushVertex()
{
	this->vertexPositionData.push_back(this->x);
	this->vertexPositionData.push_back(this->y);
	this->vertexPositionData.push_back(this->z);

	this->vertexCount++;

	return this->vertexCount - 1;
}

VAO* VAO::pushIndex(int index)
{
	this->indexData.push_back(index);

	this->indexCount++;

	return this;
}

void VAO::compile()
{
	glBindVertexArray(this->vaoId);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * 3 * sizeof(float), &this->vertexPositionData[0], GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof(unsigned int), &this->indexData[0], GL_STATIC_DRAW);

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray (0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void VAO::render()
{
	glBindVertexArray(this->vaoId);

	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}