#pragma once

#include "game_object.h"

class Mine: public GameObject
{
private:
	BBox bbox;
	int lifeTime = -1;

	const float scaleFact = 0.1;
public:
	Mine()
	{

	}

	Mine(glm::vec3 pos) : GameObject(pos, "player_vs.glslx")
	{
	}

	void loadModel()
	{
		GameObject::loadModel("Objects/watermine7.obj");
		for (auto vertex : vertices)
		{
			glm::vec3& v_pos = scaleFact * vertex.Position;
			bbox.xMin = std::min(bbox.xMin, v_pos[0]);
			bbox.xMax = std::max(bbox.xMax, v_pos[0]);
			bbox.yMin = std::min(bbox.yMin, v_pos[1]);
			bbox.yMax = std::max(bbox.yMax, v_pos[1]);
		}
	}

	void setVertices(std::vector<Vertex> vertices)
	{
		GameObject::setVertices(vertices);
			for (auto vertex : vertices)
			{
				glm::vec3& v_pos = scaleFact * vertex.Position;
				bbox.xMin = std::min(bbox.xMin, v_pos[0]);
				bbox.xMax = std::max(bbox.xMax, v_pos[0]);
				bbox.yMin = std::min(bbox.yMin, v_pos[1]);
				bbox.yMax = std::max(bbox.yMax, v_pos[1]);
			}
	}

	void update()
	{
		if (lifeTime != -2)
		{
			lifeTime--;
			if (lifeTime == -1)
				isVisible = false;
		}
	}

	BBox getBBox()
	{
		return bbox;
	}

	void setPosition(glm::vec3 position)
	{
		bbox.translate(position - pos);
		GameObject::setPosition(position);
		model = glm::scale(glm::translate(glm::mat4(), pos), glm::vec3(scaleFact));
	}

	void setLifeTime(int lifeTime)
	{
		this->lifeTime = lifeTime;
	}

	bool isExist()
	{
		return isVisible;
	}
};