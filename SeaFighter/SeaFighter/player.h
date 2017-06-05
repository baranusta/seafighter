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

	glm::vec3 gunDirection;
	float gunScaleC;

	GameObject boat, gun;

public:
	Player(glm::vec3 pos) : 
		boat(glm::vec3(0,0,0),"player_vs.glslx"),
		gun(glm::vec3(0, 3, 0), "player_vs.glslx")
	{
		rotationAngle = 0;
		acceleration = 0;

		speed = 0;

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

	bool updatePosition()
	{
		glm::vec3 oldPos = pos;
		pos += direction * speed;
		model = glm::translate(model, direction * speed);
		return pos != oldPos;
	}

	void accelerate()
	{
		acceleration += 0.0000001;
	}

	void decelerate()
	{
		acceleration -= 0.0000001;
	}

	void rotateGunTo(glm::vec3 toPosition)
	{
		glm::vec3 direction = toPosition - gunScaleC * gun.getPosition();
		direction = glm::normalize(glm::vec3(direction[0],direction[1],0));
		float angle = glm::angle(direction, gunDirection);
		if(glm::cross(direction, gunDirection)[2]>0)
			angle *= -1;
		std::cout << glm::degrees(angle) << std::endl;
		gunDirection = direction;
		gun.rotate(angle, glm::vec3(0, 1, 0));
	}

	//model matrix can only be scale matrix
	void setModel(glm::mat4 model)
	{
		float gunVsBoatScaleFactor = 0.9f;
		gunScaleC = model[0][0] * gunVsBoatScaleFactor;
		model = glm::translate(model, glm::vec3(0., 0., 0.9));
		model = glm::rotate(model, (glm::mediump_float)glm::radians(90.), glm::vec3(1, 0, 0));
		gun.setModel(glm::translate(glm::scale(model,glm::vec3(gunVsBoatScaleFactor)), gun.getPosition()));
		gun.rotate(glm::radians(90.), glm::vec3(0, 1, 0));
		boat.setModel(model);

	}
	
	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light,  GLuint textureId)
	{
		boat.draw(viewPos, cameraVp, lightVp, light, textureId);
		gun.draw(viewPos, cameraVp, lightVp, light, textureId);
	}
	
	void renderShadowMap(glm::mat4 lightVp)
	{
		boat.renderShadowMap(lightVp * model);
		gun.renderShadowMap(lightVp * model);
	}
};