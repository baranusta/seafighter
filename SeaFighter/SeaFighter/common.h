#pragma once

#include <gl\glew.h>
#include <gl\freeglut.h>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtx\rotate_vector.hpp"
#include "glm\glm\gtx\vector_angle.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <limits>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	bool hasNormal;
	Vertex() :hasNormal(false) {}
	Vertex(glm::vec3 pos) :Position(pos), hasNormal(false) {}
	Vertex(glm::vec3 pos, glm::vec3 normal) :Position(pos), Normal(normal), hasNormal(true) {}
};

struct BBox {
	float xMin, xMax, yMin, yMax;
	BBox(float xMin, float xMax, float yMin, float yMax) : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax)
	{

	}

	BBox()
	{

	}
};

inline void printError(std::string message)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << message << err << gluErrorString(err) << std::endl;
	}
}