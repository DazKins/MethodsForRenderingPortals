#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

Texture::Texture (const char* path)
{
	int nrChannels;
	unsigned char *data = stbi_load (path, &this->width, &this->height, &nrChannels, 0);

	glGenTextures (1, &this->textureHandle);

	glBindTexture (GL_TEXTURE_2D, this->textureHandle);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap (GL_TEXTURE_2D);

	stbi_image_free (data);
}

Texture::~Texture () { }

void Texture::bind ()
{
	glBindTexture (GL_TEXTURE_2D, this->textureHandle);
}