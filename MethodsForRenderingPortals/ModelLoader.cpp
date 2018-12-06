#include "ModelLoader.h"

#include <iostream>

VAO* loadObj (std::string path)
{
	std::ifstream file (path + ".obj");

	if (!file.is_open ())
	{
		std::cout << "Couldn't load file: " << path << ".obj" << std::endl;
	}

	std::string currentLine;

	VAO* vao = new VAO ();

	while (std::getline (file, currentLine))
	{
		std::stringstream stream (currentLine);

		std::string indicator = "#";

		stream >> indicator;

		if (indicator == "#")
		{
			continue;
		}
		else if (indicator == "v")
		{
			float x, y, z;

			stream >> x;
			stream >> y; 
			stream >> z;
		}
	}

	return NULL;
}