#include "ModelLoader.h"

#include <iostream>
#include <vector>

VAO* loadObj (std::string path)
{
	std::ifstream file (path + ".obj");

	if (!file.is_open ())
	{
		std::cout << "Couldn't load file: " << path << ".obj" << std::endl;
		return NULL;
	}

	VAO* vao = new VAO ();

	std::string currentLine;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

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

			vertices.push_back (glm::vec3 (x, y, z));
		}
		else if (indicator == "vn")
		{
			float nx, ny, nz;

			stream >> nx;
			stream >> ny;
			stream >> nz;

			normals.push_back (glm::vec3 (nx, ny, nz));
		}
		else if (indicator == "f")
		{
			std::string f0, f1, f2;

			stream >> f0;
			stream >> f1;
			stream >> f2;

			unsigned int v0i = 0, n0i = 0;
			unsigned int v1i = 0, n1i = 0;
			unsigned int v2i = 0, n2i = 0;

			sscanf_s (f0.c_str (), "%u//%u", &v0i, &n0i);
			sscanf_s (f1.c_str (), "%u//%u", &v1i, &n1i);
			sscanf_s (f2.c_str (), "%u//%u", &v2i, &n2i);

			glm::vec3 v0 = vertices[v0i - 1];
			glm::vec3 n0 = normals[n0i - 1];

			glm::vec3 v1 = vertices[v1i - 1];
			glm::vec3 n1 = normals[n1i - 1];

			glm::vec3 v2 = vertices[v2i - 1];
			glm::vec3 n2 = normals[n2i - 1];

			unsigned int v0ind = vao->setXYZ (v0)->setNormalXYZ (n0)->pushVertex ();
			unsigned int v1ind = vao->setXYZ (v1)->setNormalXYZ (n1)->pushVertex ();
			unsigned int v2ind = vao->setXYZ (v2)->setNormalXYZ (n2)->pushVertex ();

			vao->pushIndex (v0ind)->pushIndex (v1ind)->pushIndex (v2ind);
		}
	}

	return vao;
}