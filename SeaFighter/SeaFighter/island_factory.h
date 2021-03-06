#pragma once

#include "island.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include <cmath>  

#define ISLAND_ITER_COUNT 2

class IslandFactory
{

private:


	float unitLength;
	int xCount, yCount;

	int minR = 3;
	int maxR = 10;


	struct islandData {
		int startX;
		int startY;
		int width;
		int height;

		bool isColliding(islandData island)
		{
			bool isXCollide = startX > island.startX ? startX - island.startX < island.width : island.startX - startX < width;
			bool isYCollide = startY > island.startY ? startY - island.startY < island.height : island.startY - startY < height;
			return isXCollide & isYCollide;
		}

		void merge(islandData island)
		{
			if (startX > island.startX)
			{
				width = std::max(island.width, startX - island.startX + width);
				startX = island.startX;
			}
			else
			{
				width = std::max(width, island.startX - startX + island.width);
			}

			if (startY > island.startY)
			{
				height = std::max(island.height, startX - island.height + height);
				startY = island.startY;
			}
			else
			{
				height = std::max(height, island.startY - startY + island.height);
			}
		}
	};



	std::vector<std::vector<double>> buildIsland(int r)
	{
		std::vector<std::vector<double>> heightMap(2 * r + 1, std::vector<double>(2 * r + 1, -0.1));

		int xPos, yPos;
		for (int j = 0; j < ISLAND_ITER_COUNT * r; j++)
		{
			int offset = r * 0.4;
			xPos = (rand() % (2 * (r - offset))) + offset;
			yPos = (rand() % (2 * (r - offset))) + offset;
			float addedHeight = (rand() % r + 1) * 0.6;

			for (int k = 1; k < r + r; k++)
			{
				for (int l = 1; l < r + r; l++)
				{
					glm::vec2 centerVec(xPos, yPos);
					glm::vec2 toVec(l, k);
					//std::cout << l << " " << k << std::endl;
					float distance = glm::distance(centerVec, toVec);
					if (distance < addedHeight)
					{
						float f = sqrt(addedHeight*addedHeight - distance* distance) * unitLength;
						heightMap[k][l] += sqrt(addedHeight*addedHeight - distance* distance) * unitLength;
					}
				}
			}
		}
		return heightMap;
	}

	float normal_pdf(float x, float m, float s)
	{
		static const float inv_sqrt_2pi = 0.3989422804014327;
		float a = (x - m) / s;

		return inv_sqrt_2pi / s * std::exp(-0.5f * a * a);
	}

public:
	IslandFactory(double worldWidth, double worldHeight, double gridSize, int maxRGrid, int minRGrid)
	{
		unitLength = gridSize;
		xCount = worldWidth / unitLength;
		yCount = worldHeight / unitLength;
		minR = minRGrid;
		maxR = maxRGrid;
	}

	//It may not return exactly request island count
	std::vector<Island> getIslands(int islandCount)
	{
		if (xCount <= islandCount * maxR)
			throw " island counts are too much for this map";

		std::cout << "islands are being generated." << std::endl;
		srand(time(NULL));

		std::vector<Island> islands(islandCount);
		std::vector<islandData> islands_data;

		for (int i = 0; i < islandCount; i++)
		{
			int minSize = xCount < yCount ? xCount : yCount;
			int r, xCenter, yCenter;
			do {
				r = rand() % (maxR - minR) + minR;
				xCenter = rand() % (xCount - 1 - 2 * r) + r;
				yCenter = rand() % (yCount - 1 - 2 * r) + r;
			} while (!((yCenter + r < -1 || yCenter - r > 1) && (xCenter + r < -1 || xCenter - r > 1)));
			 


			auto heightMap = buildIsland(r);
			islandData currentIsland;
			currentIsland.startX = xCenter - r;
			currentIsland.startY = yCenter - r;
			currentIsland.width = r * 2 + 1;
			currentIsland.height = r * 2 + 1;
			bool isCollide = false;
			int index = 0;
			for (auto island_data : islands_data) {
				if (island_data.isColliding(currentIsland))
				{
					islands[index].addHeightMap(currentIsland.startX - island_data.startX, currentIsland.startY - island_data.startY, heightMap);
					island_data.merge(currentIsland);
					isCollide = true;
					break;
				}
				index++;
			}
			if (!isCollide)
			{
				islands[islands_data.size()].addHeightMap(unitLength * (xCenter - r - xCount / 2.), unitLength * (yCenter - r - yCount / 2.), heightMap);
				islands_data.push_back(currentIsland);
			}
		}
		islands.resize(islands_data.size());
		for (auto& island : islands)
			island.buildIsland(unitLength);

		std::cout << "island generation done." << std::endl;
		return islands;
	}
};