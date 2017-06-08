#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <thread>

#include "common.h"

class MeshController
{
private:
	std::vector<Vertex> vertices;

	float xStart;
	float yStart;
	float zStart;
	float xEnd;
	float yEnd;
	float zEnd;

	void sortThree(int& min, int& middle, int& max)
	{
		if (min > middle)
			std::swap(min, middle);
		if (middle > max)
			std::swap(middle, max);
	}

	void addToCell(std::unordered_map<int, std::pair<int, Vertex>>& positions, int cell, const Vertex & vert)
	{
		if (positions.find(cell) != positions.end())
		{
			glm::vec3 tempPos = vert.Position + positions[cell].second.Position * (float)positions[cell].first;
			glm::vec3 tempNormal = vert.Normal + positions[cell].second.Normal * (float)positions[cell].first;
			positions[cell].second.Position = tempPos / ((float)positions[cell].first + 1);
			positions[cell].second.Normal = tempNormal / ((float)positions[cell].first + 1);
			positions[cell].first++;
		}
		else
			positions[cell] = std::make_pair(1, vert);
	}

	int getCell(glm::vec3 position, float xSize, float ySize, float zSize,int xCellCount, int yCellCount)
	{
		return (position[0] - xStart) / xSize + ((position[1] - yStart) / ySize) * xCellCount + ((position[2] - zStart)/zSize) * xCellCount * yCellCount;
	}

public:

	MeshController()
	{

	}

	MeshController(std::vector<Vertex> & vertices) : vertices(vertices)
	{
		xStart = std::numeric_limits<float>::max();
		yStart = std::numeric_limits<float>::max();
		zStart = std::numeric_limits<float>::max();
		xEnd = std::numeric_limits<float>::min();
		yEnd = std::numeric_limits<float>::min();
		zEnd = std::numeric_limits<float>::min();
		for (auto& vertex : vertices)
		{
			xStart = std::min(xStart, vertex.Position[0]);
			yStart = std::min(yStart, vertex.Position[1]);
			zStart = std::min(zStart, vertex.Position[2]);
			xEnd = std::max(xEnd, vertex.Position[0]);
			yEnd = std::max(yEnd, vertex.Position[1]);
			zEnd = std::max(zEnd, vertex.Position[2]);
		}
	}

	void simplifyMesh(int xCellCount, int yCellCount, int zCellCount, std::function<void(std::vector<Vertex>&)> callback)
	{

		std::thread simplification([=]() {
			float xGridSize = (xEnd - xStart) / xCellCount;
			float yGridSize = (yEnd - yStart) / yCellCount;
			float zGridSize = (zEnd - zStart) / zCellCount;
			std::unordered_map<int, std::unordered_map<int, int>> faces;
			std::unordered_map<int, std::pair<int, Vertex>> positions;
			for (int i = 0; i < vertices.size(); i += 3)
			{
				int cell1 = getCell(vertices[i + 0].Position, xGridSize, yGridSize, zGridSize, xCellCount, yCellCount);
				int cell2 = getCell(vertices[i + 1].Position, xGridSize, yGridSize, zGridSize, xCellCount, yCellCount); 
				int cell3 = getCell(vertices[i + 2].Position, xGridSize, yGridSize, zGridSize, xCellCount, yCellCount);

				addToCell(positions, cell1, vertices[i + 0]);
				addToCell(positions, cell2, vertices[i + 1]);
				addToCell(positions, cell3, vertices[i + 2]);

				if (cell1 != cell2 && cell1 != cell3 && cell2 != cell3)
				{
					sortThree(cell1, cell2, cell3);
					faces[cell1][cell2] = cell3;
				}
			}
			vertices.clear();
			for (auto& each : faces)
			{
				for (auto& cont : each.second)
				{
					vertices.push_back(positions[each.first].second);
					vertices.push_back(positions[cont.first].second);
					vertices.push_back(positions[cont.second].second);
				}
			}
			callback(vertices);

		});

		simplification.detach();

	}
};