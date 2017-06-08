#pragma once
#include "game_object.h"
#include "height_field.h"
#include <algorithm>

#include <iomanip>

class Island : public GameObject, public HeightField
{
public:
	Island()
		: GameObject(glm::vec3())
	{
		color.diffuse = glm::vec4(0.4, .25, .16, 1.0);
		color.ambient = glm::vec4(0.3, .2, .1, 1.0);
		color.specConst = glm::vec4(0, 0, 0, 1.0);
	}

	void depth_first_search(int x, int y, int row_count, int col_count, std::vector<int> &boundingbox_xValues, std::vector<int> &boundingbox_yValues, std::vector<std::vector<bool>> &visited) {

		static int dx[] = { +1, 0, -1, 0 };
		static int dy[] = { 0, +1, 0, -1 };

		if (x < 0 || x == row_count) return; // out of bounds
		if (y < 0 || y == col_count) return; // out of bounds
		if (visited[x][y] || m_heightMap[x][y] < 1.0) return; // already labeled or not marked with 1 in m

		// mark the current 
		boundingbox_xValues.push_back(x);
		boundingbox_yValues.push_back(y);
		visited[x][y] = true;

		// recursively mark the neighbors
		for (int direction = 0; direction < 4; ++direction)
			depth_first_search(x + dx[direction], y + dy[direction], row_count, col_count, boundingbox_xValues, boundingbox_yValues, visited);
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
		//printHeightMap();
	}

	std::vector<BBox> getBBox()
	{
		return bBox;
	}

	void buildIsland(double gridSize)
	{
		std::list<std::vector<int>> list_ofBoundingBoxes_xValues;
		std::list<std::vector<int>> list_ofBoundingBoxes_yValues;

		std::vector<int> boundingbox_xValues;
		std::vector<int> boundingbox_yValues;

		std::vector<std::vector<bool>> visited(m_heightMap.size(), std::vector<bool>(m_heightMap[0].size(),false));
		pos = glm::vec3(m_startX + m_heightMap[0].size() / 2 * gridSize, m_startY + m_heightMap.size() / 2 * gridSize, -0.2);
		Surface surface = generateSurface(m_startX, m_startY, gridSize, gridSize, m_heightMap[0].size(), m_heightMap.size());
		indices = surface.indices;
		vertices = surface.vertices;
		
		surface.clean();
		
		setZPositions();

		for (int i = 0; i < m_heightMap.size(); i++)
		{
			for (int j = 0; j < m_heightMap[0].size(); j++)
			{
				if (m_heightMap[i][j] > 1.0 && !visited[i][j])
				{

					depth_first_search(i, j, m_heightMap.size(), m_heightMap[0].size(), boundingbox_xValues, boundingbox_yValues, visited);

					list_ofBoundingBoxes_xValues.push_back(boundingbox_xValues);
					list_ofBoundingBoxes_yValues.push_back(boundingbox_yValues);

					boundingbox_xValues.clear();
					boundingbox_yValues.clear();

				}

			}
		}
		int xmin, xmax, ymin, ymax;
		std::list<std::vector<int>>::iterator list_it_x;
		std::list<std::vector<int>>::iterator list_it_y;

		std::vector<int>::iterator vec_it;
		BBox temp_bBox;
		for (list_it_x = list_ofBoundingBoxes_xValues.begin(), list_it_y = list_ofBoundingBoxes_yValues.begin(); list_it_x != list_ofBoundingBoxes_xValues.end() && list_it_y != list_ofBoundingBoxes_yValues.end(); ++list_it_x, ++list_it_y)
		{
			xmin = *max_element(list_it_x->begin(), list_it_x->end());
			xmax = *max_element(list_it_x->begin(), list_it_x->end());
			ymin = *max_element(list_it_y->begin(), list_it_y->end());
			ymax = *max_element(list_it_y->begin(), list_it_y->end());

			temp_bBox.xMin = m_startX + gridSize * xmin;
			temp_bBox.xMax = m_startX + gridSize * xmax;
			temp_bBox.yMin = m_startY + gridSize * ymin;
			temp_bBox.yMax = m_startY + gridSize * ymax;
			bBox.push_back(temp_bBox);

		}
		
		//bBox.xMin = m_startX;
		//bBox.xMax = m_startX + gridSize * m_heightMap[0].size();
		//bBox.yMin = m_startY;
		//bBox.yMax = m_startY + gridSize * m_heightMap.size();
		m_heightMap.clear();

		setNormals();
		updateDataGPU();
	}


	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light, GLuint textureId)
	{
		//maybe first check whether it should be drawn

		passBasicsToGPU(shaderDraw, cameraVp, viewPos, light, lightVp, textureId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		printError("Island draw end");
	}

	void renderShadowMap(glm::mat4 lightVp)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		shaderShadow();
		glUniform1i(9, 1);

		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightVp));
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(3, static_cast<float>(glfwGetTime()));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

		printError("Island shadow end");
	}

protected:
	//these are the start positions in real world
	float m_startX, m_startY;
	std::vector<std::vector<double>> m_heightMap;
	std::vector<BBox> bBox;
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

		return std::vector<std::vector<double>>(height, std::vector<double>(width, -0.1f));
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

	void setZPositions()
	{
		for (int i = 0; i < m_heightMap.size(); i++)
		{
			for (int j = 0; j < m_heightMap[0].size(); j++)
			{
				vertices[i * m_heightMap[0].size() + j].Position[2] = m_heightMap[i][j];
			}
		}
	}

	void setNormals()
	{
		for (auto& vertex: vertices)
		{
			vertex.Normal = glm::vec3();
			vertex.hasNormal = true;
		}
		for (int i = 0; i < indices.size(); i += 3)
		{
			int vertex1 = indices[i];
			int vertex2 = indices[i + 1];
			int vertex3 = indices[i + 2];
			glm::vec3 edge1 = vertices[vertex2].Position - vertices[vertex1].Position;
			glm::vec3 edge2 = vertices[vertex3].Position - vertices[vertex1].Position;
			glm::vec3 Normal = glm::normalize(glm::cross(edge1, edge2));
			
			vertices[vertex1].Normal += Normal;
			vertices[vertex2].Normal += Normal;
			vertices[vertex3].Normal += Normal;
		}
		for (auto& vertex : vertices)
		{
			vertex.Normal = glm::normalize(vertex.Normal);
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

	void printHeightMap()
	{

		for (auto wid : m_heightMap)
		{
			for (auto val : wid)
				std::cout << std::setw(5) << std::setprecision(3) << val << " ";
			std::cout << std::endl;
		}
	}
};