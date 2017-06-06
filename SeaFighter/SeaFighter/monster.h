#pragma once

#include "game_object.h"
#include "quad.h"
#include <math.h>

#define SCALE_CONST 0.8f
extern glm::mat4 rotateAround(glm::mat4 model, glm::vec3 toAround, float angle, glm::vec3 rotateAxis);

class Monster : public GameObject
{
private:
	GameObject head;
	GameObject upperBody;
	GameObject lowerBody;
	GameObject tail;

	float animationSpeed;

	glm::vec3 direction;
	float speed;

public:
	Monster(glm::vec3 pos) :
		head(GameObject(glm::vec3())),
		upperBody(GameObject(glm::vec3(1., 0, 0))),
		lowerBody(GameObject(glm::vec3(2.2,0,0))),
		tail(GameObject(glm::vec3(3.4, 0, 0.0f)))
	{
		speed = 0.002;
		animationSpeed = 3;
		direction = glm::vec3(0, -1, 0);
		this->pos = pos;


		model = glm::rotate(glm::translate(model, pos), (glm::mediump_float)glm::radians(90.), glm::vec3(1, 0, 0));
		model = glm::rotate(model, (glm::mediump_float)glm::radians(90.), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(SCALE_CONST, SCALE_CONST, SCALE_CONST));

		head.loadModel("Objects/s1.obj");
		upperBody.loadModel("Objects/s2.obj");
		lowerBody.loadModel("Objects/s3.obj");
		tail.loadModel("Objects/s4.obj");

		head.setModel(model);
		upperBody.setModel(glm::translate(model, upperBody.getPosition()));
		lowerBody.setModel(glm::translate(model, lowerBody.getPosition()));
		tail.setModel(glm::translate(model, tail.getPosition()));


		color.diffuse = glm::vec4(0.01, .6, 1.0, 1.0);
		color.ambient = glm::vec4(0.0, .3, .5, 1.0);
		color.specConst = glm::vec4(0, 0, 0, 1.0);

		head.setColor(color);
		upperBody.setColor(color);
		lowerBody.setColor(color);
		tail.setColor(color);
	}

	void increaseAnimSpeed()
	{
		animationSpeed += 0.01f;
	}
	
	void slowAnimSpeed()
	{
		animationSpeed -= 0.01f;
		animationSpeed = std::max(1.0f, animationSpeed);
	}

	void updateMonsterPosition(glm::vec3 playerPos)
	{
		glm::vec3 direction = playerPos - pos;
		float angle = glm::angle(this->direction, direction);
		float rotationAngle = angle * (glm::cross(this->direction, direction)[2] > 0 ? 0.01f : -0.01f);
		this->direction = glm::rotateZ(this->direction, rotationAngle);
		pos += this->direction * speed * SCALE_CONST;
		model = glm::translate(glm::rotate(model, rotationAngle, glm::vec3(0, 0, 1)), glm::rotateZ(-this->direction, (glm::mediump_float)glm::radians(90.)) * speed);
	}

	void animate()
	{
		head.setModel(rotateAround(model, 0.5f * (upperBody.getPosition() - head.getPosition()), (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, 1, 0)));
		upperBody.setModel(glm::translate(model, upperBody.getPosition()));
		lowerBody.setModel(rotateAround(glm::translate(upperBody.getModel(), lowerBody.getPosition() - upperBody.getPosition()), 0.5f * (upperBody.getPosition() - lowerBody.getPosition()), (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, -1, 0)));
		tail.setModel(rotateAround(glm::translate(lowerBody.getModel(), tail.getPosition() - lowerBody.getPosition()), 0.5f * (lowerBody.getPosition() - tail.getPosition()), (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, -1, 0)));
	}

	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light, GLuint textureId)
	{
		head.draw(viewPos, cameraVp, lightVp, light, textureId);
		upperBody.draw(viewPos, cameraVp, lightVp, light, textureId);
		lowerBody.draw(viewPos, cameraVp, lightVp, light, textureId);
		tail.draw(viewPos, cameraVp, lightVp, light, textureId);
	}

	bool hasReached(glm::vec3 playerPos)
	{
		return  glm::distance(playerPos, pos) < 0.5;
	}

};
