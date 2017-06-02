#pragma once

#include "common.h"

class HeightField
{
protected:
	struct Surface {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void clean() 
		{
			vertices.clear();
			indices.clear();
		}
	};

	//This method does not calculate the normals
	Surface generateSurface(float startX, float startY,
						float widthGridSize,float heightGridSize, 
						int xCount, int yCount)
	{
		Surface surface;


		for (int i = 0; i < yCount; i++)
		{
			for (int j = 0; j < xCount; j++)
			{
				surface.vertices.push_back(Vertex(glm::vec3(startX + j * widthGridSize, startY + i * heightGridSize, 0.)));
			}
		}
		for (int i = 0; i < yCount - 1; i++)
		{
			for (int j = 0; j < xCount - 1; j++)
			{
				surface.indices.push_back(j + i * xCount + 0);
				surface.indices.push_back(j + i * xCount + 1);
				surface.indices.push_back(j + (i + 1) * xCount);
				surface.indices.push_back(j + i * xCount + 1);
				surface.indices.push_back(j + (i + 1) * xCount + 1);
				surface.indices.push_back(j + (i + 1) * xCount);
			}
		}
		return surface;
	}
};