#pragma once
#include "common.h"

#include <iostream>
#include <iomanip>

glm::vec4 getWorldCoordinate(glm::mat4 matrix, int xPos, int yPos, int width, int height)
{
	glm::vec4 screenPos(2 * xPos / (float)width - 1, 1 - 2 * yPos / (float)height, 0.94975, 1.0);
	glm::vec4 worldPos = matrix * screenPos;
	worldPos /= worldPos[3];
	std::cout << std::setw(8) << std::setprecision(3) << screenPos[0] << " " << screenPos[1] << " worldPos: " << worldPos[0] << " " << worldPos[1] << std::endl;
	return worldPos;
}