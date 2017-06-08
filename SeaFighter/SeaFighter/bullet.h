#pragma once
#include <math.h>

#include "quad.h"

#define BULLET_SPEED 0.016f

extern GLuint getTexture(std::string textureName);

class Bullet : public Quad
{
private:
	glm::vec3 direction;
	GLuint texture;
	int lifeStep;

	int timeToHitToIsland;
	const glm::vec3 scale = glm::vec3(0.03, 0.1, 1);

	int getClosestIslandHit(std::vector<Island>& islands)
	{
		std::list<BBox>::iterator it;

		float closestDistance = std::numeric_limits<float>::max();
		for (auto& island : islands)
		{
			for (auto& bBox : island.getBBox()) {

				closestDistance = std::min(closestDistance, getIntersectCubeT(bBox));
			}
		}
		closestDistance += BULLET_SPEED;
		return closestDistance / BULLET_SPEED;
	}

public:
	Bullet(glm::vec3 pos, glm::vec3 dir, std::vector<Island>& islands) : Quad()
	{
		this->pos = pos;
		direction = glm::normalize(dir);
		lifeStep = std::min(5000, getClosestIslandHit(islands));
		float rad = glm::angle(direction, glm::vec3(0, 1, 0));
		bool isPositive = glm::cross(direction, glm::vec3(0, 1, 0))[2] < 0;
		model = glm::translate(model, pos + glm::vec3(0, 0, 0.25));
		model = glm::rotate(model, rad, glm::vec3(0,0, isPositive ? 1 : -1));

		updatePosition(20);
		texture = getTexture("Textures/toon_map.png");
	}

	void draw(glm::mat4 cameraVp)
	{
		Quad::draw(texture, cameraVp, glm::scale(model, scale));
	}

	void updatePosition(int times = 1)
	{
		if(times == 1)
			lifeStep--;

		pos += direction * BULLET_SPEED * (float)times;
		model = glm::translate(model, glm::vec3(0,1,0) * BULLET_SPEED * (float)times);
	}

	bool isIntersectSpehere(glm::vec3 center, float radius)
	{
		float a = glm::dot(direction, direction);
		glm::vec3 L = pos - center;
		float b = 2 * glm::dot(direction, L);
		float c = glm::dot(L, L) - radius * radius;

		if (b > 0 && c > 0) return false;

		float disc = b*b - 4 * c;
		if (disc<0)
		{
			return false;
		}
		disc = sqrt(disc);
		float t = (0.5f)*(-b - disc);
		if (t >= 0 && t <= 1 * BULLET_SPEED)
		{
			return true;
		}
		else
			return false;
	}

	float getIntersectCubeT(BBox bBox)
	{
		double tmin = -std::numeric_limits<float>::min(), tmax = std::numeric_limits<float>::max();

		if (direction[0] != 0.0) {
			double tx1 = (bBox.xMin - pos[0]) / direction[0];
			double tx2 = (bBox.xMax - pos[0]) / direction[0];

			tmin = std::max(tmin, std::min(tx1, tx2));
			tmax = std::min(tmax, std::max(tx1, tx2));
		}

		if (direction[1] != 0.0) {
			double ty1 = (bBox.yMin - pos[1]) / direction[1];
			double ty2 = (bBox.yMax - pos[1]) / direction[1];

			tmin = std::max(tmin, std::min(ty1, ty2));
			tmax = std::min(tmax, std::max(ty1, ty2));
		}

		if (tmax >= tmin)
			return tmax;
		return INFINITY;
	}

	bool shouldRemove()
	{
		return lifeStep == 0;
	}
};