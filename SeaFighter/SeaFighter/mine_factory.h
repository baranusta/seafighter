#pragma once
#include "mine.h"
#include "island_factory.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

class MineFactory
{
private:
	BBox& gameArea;

	void buildEmptyCells(std::vector<int> & emptyCells, std::vector<Island>& islands, float widthGridSize, float heightGridSize, int xCount, int yCount)
	{
		std::vector<int> nonEmptyCells(xCount * yCount, true);
		for (auto& island : islands)
			for (auto bbox : island.getBBox())
			{
				for (float f = bbox.xMin; f < bbox.xMax; f += widthGridSize)
				{
					for (float s = bbox.yMin; s < bbox.yMax; s += heightGridSize)
					{
						int cellX = (f - gameArea.xMin) / widthGridSize;
						int cellY = (s - gameArea.xMin) / heightGridSize;
						if (cellX >= 0 && cellX < xCount && cellY >= 0 && cellY < yCount)
						{
							nonEmptyCells[cellX + cellY * xCount] = false;
						}
					}
				}
			}
		for (float f = -1; f < +1; f += widthGridSize)
		{
			for (float s = -1; s < +1; s += heightGridSize)
			{
				int cellX = (f - gameArea.xMin) / widthGridSize;
				int cellY = (s - gameArea.xMin) / heightGridSize;
				if (cellX >= 0 && cellX < xCount && cellY >= 0 && cellY < yCount)
				{
					nonEmptyCells[cellX + cellY * xCount] = false;
				}
			}
		}
		for (int i = 0; i < nonEmptyCells.size(); i++)
			if (nonEmptyCells[i])
				emptyCells.push_back(i);

	}

public:
	MineFactory(BBox mineArea) :gameArea(mineArea)
	{

	}

	std::vector<Mine*> generateMines(int count, std::vector<Island>& islands)
	{
		std::vector<Mine*> mines;
		Mine d_mine(glm::vec3(0, 0, 0));
		d_mine.loadModel();
		float xGridSize = d_mine.getBBox().xMax - d_mine.getBBox().xMin + 0.5;
		float yGridSize = d_mine.getBBox().yMax - d_mine.getBBox().yMin + 0.5;
		int xCount = (gameArea.xMax - gameArea.xMin) / xGridSize;
		int yCount = (gameArea.yMax - gameArea.yMin) / yGridSize;
		std::vector<int> emptyCells;

		buildEmptyCells(emptyCells, islands, xGridSize, yGridSize, xCount, yCount);

		srand(time(NULL));

		if (emptyCells.size() <= count)
			throw "too small map for mines";

		for (int i = 0; i < count; i++)
		{
			Mine* mine = new Mine(glm::vec3(0, 0, 0));
			mine->setVertices(d_mine.getVertices());
			int index = rand() % (emptyCells.size() - i);
			int cell = emptyCells[index];
			mine->setPosition(glm::vec3((cell % xCount) * xGridSize + xGridSize /2 + gameArea.xMin, (cell / xCount) * yGridSize + yGridSize / 2 + gameArea.yMin,0));
			emptyCells[index] = emptyCells[emptyCells.size() - i - 1];
			mines.push_back(mine);
		}
		return mines;
	}
};