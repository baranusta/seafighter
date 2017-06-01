#pragma once
#include "game_object.h"
#include "height_field.h"
#include <algorithm>

class Island : public GameObject, public HeightField
{
public:
	Island()
		: GameObject(glm::vec3())
	{
	}

	void addHeightMap(float startX, float startY, std::vector<std::vector<double>> heightMap)
	{
		if (m_heightMap.size())
		{
			int startx = startX;
			int starty = startY;
			std::vector<std::vector<double>> newHeightMap = getResizedEmptyHeightMap(startx, starty, heightMap);
			int xOffset = startx < 0 ? -startx : 0;
			int yOffset = starty < 0 ? -starty : 0;
			fillTheMap(xOffset, yOffset, m_heightMap, newHeightMap);

			xOffset = startx < 0 ? 0 : startx;
			yOffset = starty < 0 ? 0 : starty;
			fillTheMap(xOffset, yOffset, heightMap, newHeightMap);

			m_heightMap = newHeightMap;
		}
		else
		{
			m_startX = startX;
			m_startY = startY;
			m_heightMap = heightMap;
		}
	}

	void buildIsland(double gridSize) 
	{
		pos = glm::vec3(m_startX + m_heightMap[0].size()/2 * gridSize, m_startY + m_heightMap.size()/2 * gridSize, -0.2);
		Surface surface = generateSurface(m_startX, m_startY, gridSize, gridSize, m_heightMap[0].size(), m_heightMap.size());
		indices = surface.indices;
		vertices = surface.vertices;
		surface.clean();

		for (int i = 0; i < m_heightMap.size(); i++)
		{
			for (int j = 0; j < m_heightMap[0].size(); j++)
			{
				vertices[i * m_heightMap[0].size() + j].Position[2] = m_heightMap[i][j];
			}
		}

		updateDataGPU();
	}


	void draw(glm::mat4 mvp, glm::vec3 viewPos, float time)
	{
		//maybe first check whether it should be drawn
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		shader();
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform3fv(2, 1, glm::value_ptr(viewPos));
		
		//std::cout << glfwGetTime()<<std::endl;
		glUniform1f(3, static_cast<float>(glfwGetTime()));
		//glUniform1f(4, startX);
		//glUniform1f(5, startY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		// Bind the texture to slot 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		printError("Island draw end");
	}

protected: 
	//these are the start positions in real world
	float m_startX, m_startY;
	std::vector<std::vector<double>> m_heightMap;
private:
	std::vector<unsigned int> indices;
	GLuint elementbuffer;

	std::vector<std::vector<double>> getResizedEmptyHeightMap(int startX, int startY, const std::vector<std::vector<double>>& heightMap)
	{
		int height, width;
		if (startX < 0)
			width = std::max(heightMap[0].size(), m_heightMap[0].size() - startX);
		else
			width = std::max(m_heightMap[0].size(), heightMap[0].size() + startX);

		if (startY < 0)
			height = std::max(heightMap.size(), m_heightMap.size() - startY); 
		else
			height = std::max(m_heightMap.size(), heightMap[0].size() + startY); 

		return std::vector<std::vector<double>>(height, std::vector<double>(width));
	}

	void fillTheMap(int xOffset, int yOffset, const std::vector<std::vector<double>>& fromHeightMap, std::vector<std::vector<double>>& toHeightMap) 
	{
		for (int i = 0; i < fromHeightMap.size(); i++)
		{
			for (int j = 0; j < fromHeightMap[0].size(); j++)
			{
				toHeightMap[yOffset + i][xOffset + j] = fromHeightMap[i][j];
			}
		}
	}

	void updateDataGPU()
	{
		GameObject::updateDataGPU();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


};