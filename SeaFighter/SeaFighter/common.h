#pragma once

#include <gl\glew.h>
#include <gl\freeglut.h>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtx\rotate_vector.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"

#include <vector>
#include <iostream>
#include <string>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	bool hasNormal;
	Vertex(glm::vec3 pos) :Position(pos), hasNormal(false) {}
	Vertex(glm::vec3 pos, glm::vec3 normal) :Position(pos), Normal(normal), hasNormal(true) {}
};

inline void printError(std::string message)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << message << err << gluErrorString(err) << std::endl;
	}
}