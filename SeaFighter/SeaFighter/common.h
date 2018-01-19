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
#include <algorithm>

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
		xMin = +1000;
		yMin = +1000;
		xMax = -1000;
		yMax = -1000;

	}

	void translate(glm::vec3 vec)
	{
		xMin += vec[0];
		xMax += vec[0];
		yMin += vec[1];
		yMax += vec[1];
	}

	bool isColliding(BBox bbox)
	{
		bool isXCollide = xMin > bbox.xMin && xMin<bbox.xMax || xMax<bbox.xMax && xMax>bbox.xMax || xMin< bbox.xMin && xMax> bbox.xMax;
		bool isYCollide = yMin > bbox.yMin && yMin<bbox.yMax || yMax<bbox.yMax && yMax>bbox.yMax || yMin< bbox.yMin && yMax> bbox.yMax;
		return isXCollide & isYCollide;
	}

	bool isColliding(std::vector<glm::vec3>& points)
	{
		for (const auto& point : points)
		{
			bool isXCollide = xMin > point[0] && xMin<point[0];
			bool isYCollide = xMin > point[1] && xMin<point[1];
			if (isXCollide & isYCollide)
				return true;
		}
		return false;
	}
};

inline void printError(std::string message)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << message << err << gluErrorString(err) << std::endl;
	}
}