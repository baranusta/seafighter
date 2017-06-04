#pragma once

#include "naval_mine.h"
#include "island.h"
#include <stdlib.h>     
#include <time.h>  
#include <cmath> 

class MineFactory
{
private:

	struct mineData {
		int startX;
		int startY;
		int width; //This just to make the size of the plane
		int height; //The size of the plane

		bool isColliding(mineData navalMine) {

			bool isXCollide = startX > navalMine.startX ? startX - navalMine.startX < navalMine.width : navalMine.startX - startX < width;
			bool isYCollide = startY > navalMine.startY ? startY - navalMine.startY < navalMine.height : navalMine.startY - startY < height;
			return isXCollide & isYCollide;
		}

	};
	std::vector<std::vector<double>> buildMines(int r) {

		std::vector<std::vector<double>> heightMap(2 * r + 1, std::vector<double>(2 * r + 1, -0.));
		int xPos, yPos;
		for (int j = 0; j < ISLAND_ITER_COUNT * r; j++)
		{
			xPos = (rand() % (r * 2 + 1));
			yPos = (rand() % (r * 2 + 1));
			float addedHeight = unitLength * (rand() % (int)(r * 0.6));

			for (int k = 1; k < r + r; k++)
			{
				for (int l = 1; l < r + r; l++)
				{
					glm::vec2 centerVec(xPos, yPos);
					glm::vec2 toVec(l, k);
					//std::cout << l << " " << k << std::endl;
					float distance = glm::distance(centerVec, toVec);
					if (distance < addedHeight)
						heightMap[k][l] += sqrt(addedHeight*addedHeight - distance* distance);
				}
			}
		}
		return heightMap;
	}

}




public:
	MineFactory(double worlwide, double  gridSize) {


	}




};