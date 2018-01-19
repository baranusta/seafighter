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
		Color boatColor;
		Color gunColor;

		gunColor.diffuse = glm::vec4(0.1, .1, .1, 1.0);
		gunColor.ambient = glm::vec4(0.1, .1, .1, 1.0);
		gunColor.specConst = glm::vec4(1,1, 1, 1.0);\

		boatColor.diffuse = glm::vec4(0.2, .1, .2, 1.0);
		boatColor.ambient = glm::vec4(0.2, .2, .1, 1.0);
		boatColor.specConst = glm::vec4(0, 0, 0, 1.0);

		boat.setColor(boatColor);
		gun.setColor(gunColor);


		rotationAngle = 0;
		acceleration = 0;

		speed = 0;

		direction = glm::vec3(0,1,0);
		gunDirection = glm::vec3(0, 1, 0);
		
		this->pos = pos;
	}

	bool loadModel(std::string boat_name, std::string gun_name)
	{
		bool result = boat.loadModel(boat_name) && gun.loadModel(gun_name);
		return result;
	}

	virtual std::vector<glm::vec3> getPoints()
	{
		return boat.getPoints();
	}

	void rotate(float angle)
	{
		float rad = glm::radians(angle);
		rotationAngle += rad;
		direction = glm::rotate(direction, -rad, glm::vec3(0, 0, 1));
		model = glm::rotate(model, -rad, glm::vec3(0, 0, 1));
	}

	void reset()
	{
		setPosition(glm::vec3(0, 0, 0));
		direction = glm::rotate(direction, -rotationAngle, glm::vec3(0, 0, 1));
		rotationAngle = 0;
		model = glm::mat4();
	}

	float getRotation()
	{
		return rotationAngle;
	}

	void updateSpeed()
	{
		float waterForce = 0.000006f;

		acceleration -= waterForce;
		speed += acceleration;
		if (speed > 0.01)
		{
			speed = 0.01;
		}
		if (speed < 0) {
			speed = 0;
			acceleration = 0;
		}
	}

	bool updatePosition()
	{
		glm::vec3 oldPos = pos;
		pos += direction * speed;
		model = glm::translate(model, glm::vec3(0, 1, 0) * speed);
		return pos != oldPos;
	}

	void updatePosition(glm::vec3 pos)
	{
		this->pos += pos;
		model = glm::translate(model, pos);
	}

	void accelerate()
	{
		acceleration += 0.00001;
	}

	void decelerate()
	{
		acceleration -= 0.0001;
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

	glm::vec3 getGunDirection()
	{
		return glm::rotateZ(gunDirection, -rotationAngle);
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
		boat.draw(viewPos, cameraVp, lightVp * model, light, textureId);
		gun.draw(viewPos, cameraVp, lightVp * model, light, textureId);
	}
	
	void renderShadowMap(glm::mat4 lightVp, glm::mat4 parentModel)
	{
		boat.renderShadowMap(lightVp, model);
		gun.renderShadowMap(lightVp, model);
	}
};