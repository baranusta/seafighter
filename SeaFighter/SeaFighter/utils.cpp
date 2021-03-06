#include "common.h"

#include <iostream>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


glm::vec4 getWorldCoordinate(glm::mat4 matrix, int xPos, int yPos, int width, int height, float z = 0.94975)
{
	glm::vec4 screenPos(2 * xPos / (float)width - 1, 1 - 2 * yPos / (float)height, z, 1.0);
	glm::vec4 worldPos = matrix * screenPos;
	worldPos /= worldPos[3];
	//std::cout << std::setw(8) << std::setprecision(3) << screenPos[0] << " " << screenPos[1] << " worldPos: " << worldPos[0] << " " << worldPos[1] << std::endl;
	return worldPos;
}

float getOriginsZ(glm::mat4 matrix)
{
	glm::vec4 worldPosition(0, 0, 0, 1);
	glm::vec4 screenPos = matrix * worldPosition;
	screenPos /= screenPos[3];
	return screenPos[2];
}

GLuint getTexture(std::string textureName,GLuint s_outbehaviour = GL_CLAMP_TO_EDGE, GLuint t_outbehaviour= GL_CLAMP_TO_EDGE)
{
	GLuint texture;
	int width, height, channels;
	stbi_uc* pixels = stbi_load(textureName.c_str(), &width, &height, &channels, 3);

	// Create Texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Upload pixels into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Set behaviour for when texture coordinates are outside the [0, 1] range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_outbehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_outbehaviour);

	// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

glm::mat4 rotateAround(glm::mat4 model, glm::vec3 toAround, float angle,glm::vec3 rotateAxis)
{
	model = glm::translate(model, toAround);
	model = glm::rotate(model, angle, rotateAxis);
	return glm::translate(model, -toAround);
}
