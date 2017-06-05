#pragma once

#include "naval_mine.h"
#include "island_factory.h"
#include "island.h"
#include <stdlib.h>     
#include <time.h>  
#include <cmath> 


#define MINE_ITER_COUNT 2

class MineFactory
{
private:

	float unitLength;
	int xCount, yCount;

	int minR = 3;
	int maxR = 10;

	struct mineData {

		int startX;
		int startY;
		//int width; //This just to make the size of the plane
		//int height; //The size of the plane. It might not be here

		bool isColliding(mineData navalMine) {

			//bool isXCollide = startX > navalMine.startX ? startX - navalMine.startX < navalMine.width : navalMine.startX - startX < width;
			//bool isYCollide = startY > navalMine.startY ? startY - navalMine.startY < navalMine.height : navalMine.startY - startY < height;
			//return isXCollide & isYCollide;
			return false;
		}

		bool isCollidingMineAndIsland(std::vector<Island> Islands, mineData navalMine) {

			bool isXIslandCollide;
			bool isYIslandCollide;
			bool isXMineCollide;
			bool isYMineCollide;

			return false;
		}


	};

	
	//std::vector<std::vector<double>> buildMine(int r)
	//{
	//	std::vector<std::vector<double>> enemiesMap(2 * r + 1, std::vector<double>(2 * r + 1, -0.1));

	//	int xPos, yPos;
	//	for (int j = 0; j < ISLAND_ITER_COUNT * r; j++)
	//	{
	//		int offset = r * 0.4;
	//		xPos = (rand() % (2 * (r - offset))) + offset;
	//		yPos = (rand() % (2 * (r - offset))) + offset;
	//		//float addedHeight = (rand() % r + 1) * 0.6;

	//		for (int k = 1; k < r + r; k++)
	//		{
	//			for (int l = 1; l < r + r; l++)
	//			{
	//				glm::vec2 centerVec(xPos, yPos);
	//				//glm::vec2 toVec(l, k);
	//				//std::cout << l << " " << k << std::endl;
	//				//float distance = glm::distance(centerVec, toVec);
	//				//if (distance < addedHeight)
	//				{
	//					float f = sqrt(addedHeight*addedHeight - distance* distance) * unitLength;
	//					heightMap[k][l] += sqrt(addedHeight*addedHeight - distance* distance) * unitLength;
	//					enemiesMap[k][l] += 
	//				}
	//			}
	//		}
	//	}
	//	return heightMap;
	//}




public:
	MineFactory(double worldWidth, double worldHeight, double  gridSize, int maxRGrid, int minRGrid)
	{

		unitLength = gridSize;
		xCount = worldWidth / unitLength; //Number of boxes of the grid x-axis
		yCount = worldHeight / unitLength;//Number of boxes in the grid y-axis
		minR = minRGrid;
		maxR = maxRGrid;
		//std::vector<Island> restultsIland;
	}

	//Main method that will get the Enemies
	std::vector<Mine> buildEnemies(std::vector<Island> Islands, int enemiesCount) {
		if (xCount <= enemiesCount * maxR)
			throw "Enemies are too many for this map";

		std::cout << "Enemies are being generated" << std::endl;
		srand(time(NULL));

		std::vector<Mine> mines(enemiesCount);
		std::vector<mineData> mines_data;

		for (int i = 0; i < enemiesCount; i++)
		{
			int minSize = xCount < yCount ? xCount : yCount;

			//int r = rand() % (maxR - minR) + minR;
			int xCenter = rand() % maxR;
			int yCenter = rand() % maxR;


			mineData currentMine; //Too away the points
			currentMine.startX = xCenter;
			currentMine.startY = yCenter;

			mines[i].createEnemiesMap(currentMine.startX, currentMine.startY);
			
		}

		std::cout << "Naval enemies generation done." << std::endl;
		return mines;


		//for (int i = 0; i < enemiesCount; i++)
		//{
		//	int minSize = xCount < yCount ? xCount : yCount;

		//	int r = rand() % (maxR - minR) + minR;
		//	int xCenter = rand() % (xCount - 1 - 2 * r) + r;
		//	int yCenter = rand() % (yCount - 1 - 2 * r) + r;

		//	//auto enemiesMap = buildEnemies();
		//	mineData currentMine; //Too away the points
		//	currentMine.startX = xCenter - r;
		//	currentMine.startY = yCenter - r;
		//	//currentIsland.width = r * 2 + 1;
		//	//currentIsland.height = r * 2 + 1;

		//	bool isCollide = false;
		//	int index = 0;
		//	for (auto mine_data : mines_data) {
		//		if (mine_data.isColliding(currentMine) & mine_data.isCollidingMineAndIsland(Islands, currentMine))
		//		{
		//			//IT should go in other place the mine
		//			//islands[index].addHeightMap(currentIsland.startX - island_data.startX, currentIsland.startY - island_data.startY, heightMap);
		//			//island_data.merge(currentIsland);
		//			//mines[index].createEnemiesMap(currentMine.startX - mine_data.startX, currentMine.startY - mine_data.startY);
		//			isCollide = false;
		//			mines[index];
		//			std::cout << "There was a collision of island and mine" << std::endl;
		//			break;
		//		}
		//		index++;
		//	}
		//	//If does not collide then add the Naval Mine to the vector
		//	if (!isCollide)
		//	{
		//		//islands[islands_data.size()].addHeightMap(unitLength * (xCenter - r - xCount / 2.), unitLength * (yCenter - r - yCount / 2.), heightMap);
		//		//islands_data.push_back(currentIsland);
		//		//mines[mines_data.size()]
		//		mines[index].createEnemiesMap(currentMine.startX, currentMine.startY);
		//		mines_data.push_back(currentMine);
		//	}
		//}
		////islands.resize(islands_data.size());
		////mines.resize(mines_data.size());
		////After finished the selecting the Islands build their positions
		//for (auto& mine : mines)
		//	mine.buildMine(unitLength);
		//
		//std::cout << "island generation done." << std::endl;
		//return mines;

		
	}



	void compareIslandAndMines() {

	}

	void compareIslandCollisions() {

		std::vector<Island> something;

	}



};

