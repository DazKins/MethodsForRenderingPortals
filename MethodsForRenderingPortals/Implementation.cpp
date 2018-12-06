#include "Implementation.h"

Implementation::Implementation (Input* input, Window* window)
{
	this->window = window;
	this->input = input;

	this->camera = new Camera (this->window);
	this->player = new Player (this->input, this->window, this->camera);
}

void Implementation::render ()
{

}

void Implementation::tick ()
{
	player->tick ();
}

Implementation::~Implementation ()
{
}