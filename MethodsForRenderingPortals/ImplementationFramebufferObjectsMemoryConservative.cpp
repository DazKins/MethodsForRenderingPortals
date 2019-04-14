#include "ImplementationFramebufferObjectsMemoryConservative.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "ModelLoader.h"

ImplementationFramebufferObjectsMemoryConservative::ImplementationFramebufferObjectsMemoryConservative (Input* input, Window* window, int textureSize, int maxRecursionDepth, bool manualCamera)
	: Implementation (input, window, maxRecursionDepth, manualCamera)
{
	this->textureSize = textureSize;

	this->portalFrameBuffers = (unsigned int*)malloc (sizeof (unsigned int) * 2);
	this->portalTextures = (unsigned int*)malloc (sizeof (unsigned int) * 2);

	for (int i = 0; i < 2; i++)
	{
		auto portal = ImplementationFramebufferObjectsMemoryConservative::createPortalFrameBuffer (textureSize);
		this->portalFrameBuffers[i] = std::get<0> (portal);
		this->portalTextures[i] = std::get<1> (portal);
	}
}

std::tuple<unsigned int, unsigned int> ImplementationFramebufferObjectsMemoryConservative::createPortalFrameBuffer (int textureSize)
{
	unsigned int fboId;
	glGenFramebuffers (1, &fboId);
	glBindFramebuffer (GL_FRAMEBUFFER, fboId);

	unsigned int textureId;
	glGenTextures (1, &textureId);
	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, textureSize, textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	unsigned int rbo;
	glGenRenderbuffers (1, &rbo);
	glBindRenderbuffer (GL_RENDERBUFFER, rbo);
	glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, textureSize, textureSize);
	glBindRenderbuffer (GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Portal framebuffer is incomplete" << std::endl;

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	return std::make_tuple (fboId, textureId);
}

ImplementationFramebufferObjectsMemoryConservative::~ImplementationFramebufferObjectsMemoryConservative ()
{
	//for (unsigned int i : this->portalFrameBuffers)
	//	glDeleteFramebuffers (1, &i);
}

glm::mat4 ImplementationFramebufferObjectsMemoryConservative::generateCustomProjection (glm::mat4 oldViewMatrix, glm::mat4 currentViewMatrix, Portal * inPortal, Portal * outPortal)
{
	glm::vec3* outWorldVertices = outPortal->getWorldVertices ();

	glm::vec3 TL = currentViewMatrix * glm::vec4 (outWorldVertices[1], 1.0f);
	glm::vec3 BR = currentViewMatrix * glm::vec4 (outWorldVertices[3], 1.0f);

	glm::mat4 rotation = glm::mat3 (inPortal->getInvToWorld ()) * glm::mat3 (glm::inverse (oldViewMatrix));

	TL = rotation * glm::vec4 (TL, 1.0f);
	BR = rotation * glm::vec4 (BR, 1.0f);

	float common_z = abs (BR.z);
	static float near_z = 0.1f;
	float ratio = near_z / common_z;

	float left = TL.x * ratio;
	float right = BR.x * ratio;
	float bottom = BR.y * ratio;
	float top = TL.y * ratio;

	glm::mat4 frustumProjection = glm::frustum (right, left, bottom, top, near_z, 100.0f);

	return frustumProjection * rotation;
}

bool alternator = 0;

void ImplementationFramebufferObjectsMemoryConservative::renderFromPortalPerspective (glm::mat4 translationMatrix, Portal * inPortal, Portal * outPortal,
	unsigned int* inPortalTextures, unsigned int* inPortalFrameBuffers, Level * level, int textureSize, int maxRecursionDepth, int cutoff,
	glm::mat4 * viewOperators)
{
	Shader::PORTAL_CLIP.bind ();
	Shader::setUniform ("portalPosition", outPortal->getPosition ());
	Shader::setUniform ("portalNormal", outPortal->getNormal ());

	glViewport (0, 0, textureSize, textureSize);

	glm::mat4 thisViewMatrix = translationMatrix * viewOperators[maxRecursionDepth - 1];

	for (int i = maxRecursionDepth - 1; i > cutoff; i--)
	{
		glBindFramebuffer (GL_FRAMEBUFFER, inPortalFrameBuffers[alternator]);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 nextViewMatrix = translationMatrix * viewOperators[i - 1];

		glm::mat4 projectionMatrix = generateCustomProjection (nextViewMatrix, thisViewMatrix, inPortal, outPortal);

		Shader::PORTAL_CLIP.bind ();
		Shader::setUniform ("viewMatrix", thisViewMatrix);
		Shader::setUniform ("projectionMatrix", projectionMatrix);
		level->render ();

		if (i < maxRecursionDepth - 1)
		{
			Shader::PORTAL_FRAMEBUFFER_OBJECT.bind ();
			Shader::setUniform ("viewMatrix", thisViewMatrix);
			Shader::setUniform ("projectionMatrix", projectionMatrix);
			Shader::setUniform ("modelMatrix", inPortal->getToWorld ());

			glBindTexture (GL_TEXTURE_2D, inPortalTextures[!alternator]);
			inPortal->model->render ();
		}

		thisViewMatrix = nextViewMatrix;
		alternator = !alternator;
	}
}

void ImplementationFramebufferObjectsMemoryConservative::renderFromPortalPerspective (glm::mat4 translationMatrix, Portal * inPortal, Portal * outPortal,
	unsigned int* inPortalTextures, unsigned int* inPortalFrameBuffers, Level * level, int textureSize, int maxRecursionDepth,
	glm::mat4 * viewOperators)
{
	renderFromPortalPerspective (translationMatrix, inPortal, outPortal, inPortalTextures, inPortalFrameBuffers, level, textureSize, maxRecursionDepth, 0, viewOperators);
}

void ImplementationFramebufferObjectsMemoryConservative::render ()
{
	Shader::DEFAULT.bind ();
	Shader::setUniform ("viewMatrix", this->camera->getViewMatrix ());
	Shader::setUniform ("projectionMatrix", defaultProjection);
	Shader::setUniform ("modelMatrix", glm::mat4 (1.0f));
	level->render ();

	this->renderFromPortalPerspective (this->camera->getTranslationMatrix (), portal1, portal2, portalTextures, portalFrameBuffers, level, textureSize, maxRecursionDepth,
		portal1ViewOperators);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glViewport (0, 0, this->window->getWidth (), this->window->getHeight ());

	Shader::PORTAL_FRAMEBUFFER_OBJECT.bind ();
	Shader::setUniform ("viewMatrix", this->camera->getViewMatrix ());
	Shader::setUniform ("projectionMatrix", defaultProjection);
	Shader::setUniform ("modelMatrix", portal1->getToWorld ());

	glBindTexture (GL_TEXTURE_2D, this->portalTextures[!alternator]);
	portal1->model->render ();

	this->renderFromPortalPerspective (this->camera->getTranslationMatrix (), portal2, portal1, portalTextures, portalFrameBuffers, level, textureSize, maxRecursionDepth,
		portal2ViewOperators);

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glViewport (0, 0, this->window->getWidth (), this->window->getHeight ());

	Shader::PORTAL_FRAMEBUFFER_OBJECT.bind ();
	Shader::setUniform ("viewMatrix", this->camera->getViewMatrix ());
	Shader::setUniform ("projectionMatrix", defaultProjection);
	Shader::setUniform ("modelMatrix", portal2->getToWorld ());

	glBindTexture (GL_TEXTURE_2D, this->portalTextures[!alternator]);
	portal2->model->render ();
}

void ImplementationFramebufferObjectsMemoryConservative::tick ()
{
	Implementation::tick ();
}