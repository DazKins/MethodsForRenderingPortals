#pragma once

#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Level.h"

class Player;

struct Portal
{
public:
	static const float PORTAL_SIZE;
	static const glm::vec3 vertices[];
	static const glm::vec3 normal;
	
	void setToWorld (glm::mat4 toWorld);
	glm::mat4 getToWorld ();
	glm::mat4 getInvToWorld ();

	glm::vec3* getWorldVertices ();

	VAO *model;
	void generatePortalMesh ();

	glm::vec3 getPosition ();
	glm::vec3 getNormal ();
private:
	glm::mat4 toWorld;
	glm::mat4 invToWorld;

	glm::vec3 worldVertices[4];
};

class Implementation
{
public:
	Implementation (Input* input, Window* window, int maxRecursionDepth, bool manualCamera);
	~Implementation ();

	virtual void tick ();
	virtual void render ();

	static glm::mat4 getNewCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);
	static glm::mat4 getOldCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);

	Portal* getPortal1 () const;
	Portal* getPortal2 () const;

protected:
	Level *level;
	Input *input;
	Window *window;
	Camera *camera;
	Player *player;

	Portal *portal1;
	Portal *portal2;

	int maxRecursionDepth;

	glm::mat4 * portal1ViewOperators;
	glm::mat4 * portal2ViewOperators;
};
