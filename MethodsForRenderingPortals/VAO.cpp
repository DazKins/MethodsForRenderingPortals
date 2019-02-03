#include "VAO.h"

#include <glad/glad.h>
#include <iostream>

VAO::VAO ()
{
	glGenVertexArrays (1, &this->vaoId);

	this->vertexCount = 0;
}

VAO::~VAO ()
{

}

VAO* VAO::setX (float x)
{
	this->x = x;
	return this;
}

VAO* VAO::setY (float y)
{
	this->y = y;
	return this;
}

VAO* VAO::setZ (float z)
{
	this->z = z;
	return this;
}

VAO* VAO::setXYZ (float x, float y, float z)
{
	this->setX (x);
	this->setY (y);
	return this->setZ (z);
}

VAO* VAO::setXYZ (glm::vec3 xyz)
{
	this->setX (xyz.x);
	this->setY (xyz.y);
	return this->setZ (xyz.z);
}

VAO* VAO::setNormalX (float x)
{
	this->nx = x;
	return this;
}

VAO* VAO::setNormalY (float y)
{
	this->ny = y;
	return this;
}

VAO* VAO::setNormalZ (float z)
{
	this->nz = z;
	return this;
}

VAO* VAO::setNormalXYZ (float x, float y, float z)
{
	this->setNormalX (x);
	this->setNormalY (y);
	return this->setNormalZ (z);
}

VAO* VAO::setNormalXYZ (glm::vec3 xyz)
{
	this->setNormalX (xyz.x);
	this->setNormalY (xyz.y);
	return this->setNormalZ (xyz.z);
}

VAO* VAO::setU (float u)
{
	this->u = u;
	return this;
}

VAO* VAO::setV (float v)
{
	this->v = v;
	return this;
}

VAO* VAO::setUV (float u, float v)
{
	this->setU (u);
	return this->setV (v);
}

int VAO::pushVertex ()
{
	this->vertexData.push_back (this->x);
	this->vertexData.push_back (this->y);
	this->vertexData.push_back (this->z);

	this->vertexData.push_back (this->nx);
	this->vertexData.push_back (this->ny);
	this->vertexData.push_back (this->nz);

	this->vertexData.push_back (this->u);
	this->vertexData.push_back (this->v);

	this->vertexCount++;

	return this->vertexCount - 1;
}

VAO* VAO::pushIndex (int index)
{
	this->indexData.push_back (index);

	this->indexCount++;

	return this;
}

void VAO::compile ()
{
	glBindVertexArray (this->vaoId);

	unsigned int VBO;
	glGenBuffers (1, &VBO);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glBufferData (GL_ARRAY_BUFFER, this->vertexCount * 8 * sizeof (float), &this->vertexData[0], GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers (1, &EBO);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof (unsigned int), &this->indexData[0], GL_STATIC_DRAW);

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);

	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(3 * sizeof (float)));
	glEnableVertexAttribArray (1);

	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6 * sizeof (float)));
	glEnableVertexAttribArray (2);

	glBindBuffer (GL_ARRAY_BUFFER, 0);

	glBindVertexArray (0);

	this->compiled = true;
}

bool VAO::isCompiled ()
{
	return this->compiled;
}

void VAO::render ()
{
	if (!this->compiled)
		this->compile ();


	glBindVertexArray (this->vaoId);

	glDrawElements (GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray (0);
}