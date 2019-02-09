#pragma once

#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Player.h"
#include "Level.h"

struct Portal
{
	static const float PORTAL_SIZE;
	static const glm::vec3 vertices[];
	static const glm::vec3 normal;

	glm::mat4 toWorld;
	VAO *model;
	void generatePortalMesh ();

	glm::vec3 getPosition ();
	glm::vec3 getNormal ();
};

class Implementation
{
public:
	Implementation (Input* input, Window* window);
	~Implementation ();

	virtual void tick ();
	virtual void render ();

	static glm::mat4 getNewCameraView (glm::mat4 currentViewMatrix, Portal* inPortal, Portal* outPortal);

protected:
	Level *level;
	Input *input;
	Window *window;
	Camera *camera;
	Player *player;

	Portal *portal1;
	Portal *portal2;

	static const int MAX_RECURSION_DEPTH;
};
