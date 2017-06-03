#pragma once
#include "game_object.h"

#include "glm/glm/gtc/matrix_transform.hpp"

class Player: public GameObject
{
private:
	float rotationAngle;
	float acceleration;

	glm::vec3 direction;
	float speed;
	glm::mat4 mvp;

	glm::vec3 gunDirection;
	float gunAngle;

	GameObject boat, gun;

public:
	Player(glm::vec3 pos) : 
		boat(glm::vec3(0,0,0),"player_vs.glslx"),
		gun(glm::vec3(0, 0.15, 0.1), "player_vs.glslx")
	{
		rotationAngle = 0;
		acceleration = 0;

		speed = 0;
		gunAngle = 0;

		direction = glm::vec3(0,1,0);
		gunDirection = glm::vec3(0, 1, 0);
		
		this->pos = pos;
	}

	bool loadModel(std::string boat_name, std::string gun_name)
	{
		return boat.loadModel(boat_name) && gun.loadModel(gun_name);
	}

	void rotate(float angle)
	{
		float rad = glm::radians(angle);
		rotationAngle += rad;
		direction = glm::rotate(direction, -rad, glm::vec3(0, 0, 1));
	}

	float getRotation()
	{
		return rotationAngle;
	}

	void updateSpeed()
	{
		float waterForce = 0.000000002f;
		if(speed<0.0005)
			speed += acceleration - waterForce;
		if (speed < 0)
			speed = 0;
		if (acceleration > 0)
			acceleration -= waterForce;
	}

	void updatePosition()
	{
		pos += direction * speed;
	}

	void accelerate()
	{
		acceleration += 0.00000001;
	}

	void decelerate()
	{
		acceleration -= 0.00000001;
	}

	void rotateGunTo(glm::vec3 toPosition)
	{
		glm::vec3 direction = toPosition - gun.getPosition();
		direction = glm::normalize(glm::vec3(direction[0],direction[1],0));
		if(glm::cross(direction, gunDirection)[2]<0)
			gunAngle += glm::angle(direction, gunDirection);
		else
			gunAngle -= glm::angle(direction, gunDirection);

		float lim = glm::radians(360.);
		gunAngle = gunAngle >= lim? gunAngle - lim: gunAngle<0? lim + gunAngle : gunAngle;
		std::cout << glm::degrees(gunAngle) << std::endl;
		gunDirection = direction;
	}

	void setMvp(glm::mat4 mvp)
	{
		this->mvp = glm::rotate(mvp, (glm::mediump_float)glm::radians(90.), glm::vec3(1, 0, 0));
		this->mvp = glm::rotate(this->mvp, (glm::mediump_float)glm::radians(180.), glm::vec3(0, 1, 0));
	}
	
	void draw(glm::vec3 viewPos, glm::vec3 light)
	{
		//this->mvp = glm::rotate(mvp, (glm::mediump_float)glm::radians(0.01), glm::vec3(0, 1, 0));
		boat.draw(mvp, viewPos, light);
		gun.draw(glm::rotate(glm::translate(mvp,glm::vec3(0,3,1)), gunAngle,glm::vec3(0,1,0)), viewPos, light);
	}

};