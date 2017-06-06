#pragma once

#include "game_object.h"
#include "quad.h"
#include <math.h>

#define SCALE_CONST 0.2f

extern GLuint getTexture(std::string textureName);
class Monster : public GameObject
{
private:
	Quad head;
	Quad upperBody;
	Quad lowerBody;
	Quad tail;

	GLuint texture;

	float animationSpeed;

	glm::vec3 direction;
	float speed;

public:
	Monster(glm::vec3 pos) :
		//head(GameObject(glm::vec3())),
		//upperBody(GameObject(glm::vec3())),
		//lowerBody(GameObject(glm::vec3())),
		//tail(GameObject(glm::vec3()))

		head(Quad()),
		upperBody(Quad(glm::vec3(0, -1.75, 0))),
		lowerBody(Quad(glm::vec3(0, -3.5, 0))),
		tail(Quad(glm::vec3(0, -5.25, 0)))
	{
		speed = 0.002;
		animationSpeed = 2;
		direction = glm::vec3(0, -1, 0);
		this->pos = pos;

		model = glm::rotate(glm::translate(model, pos), (glm::mediump_float)glm::radians(180.), glm::vec3(0, 0, 1));
		texture = getTexture("Textures/toon_map.png");
		//toPos = glm::rotate(toPos, (glm::mediump_float)glm::radians(90.), glm::vec3(0, 0, -1));
		//toPos = glm::rotate(toPos, (glm::mediump_float)glm::radians(90.), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(SCALE_CONST, SCALE_CONST, SCALE_CONST));

		//head.loadModel("Objects/head.obj");
		//upperBody.loadModel("Objects/upperbody.obj");
		//lowerBody.loadModel("Objects/lowerbody.obj");
		//tail.loadModel("Objects/tail.obj");

		head.setModel(model);
		upperBody.setModel(glm::translate(model, upperBody.getPosition()));
		lowerBody.setModel(glm::translate(model, lowerBody.getPosition()));
		tail.setModel(glm::translate(model, tail.getPosition()));
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
		model = glm::translate(glm::rotate(model, rotationAngle, glm::vec3(0, 0, 1)), -this->direction * speed);
	}

	void animate()
	{
		//glfwGetTime();
		head.setModel(glm::rotate(model, (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, 0, -1)));
		upperBody.setModel(glm::translate(model, upperBody.getPosition()));
		lowerBody.setModel(glm::rotate(glm::translate(upperBody.getModel(), lowerBody.getPosition() - upperBody.getPosition()), (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, 0, 1)));
		tail.setModel(glm::rotate(glm::translate(lowerBody.getModel(), tail.getPosition() - lowerBody.getPosition()), (glm::mediump_float)glm::cos(glfwGetTime() * animationSpeed) / 10, glm::vec3(0, 0, 1)));
	}

	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light, GLuint textureId)
	{
		//head.draw(viewPos, cameraVp, lightVp, light, textureId);
		//upperBody.draw(viewPos, cameraVp, lightVp, light, textureId);
		//lowerBody.draw(viewPos, cameraVp, lightVp, light, textureId);
		//tail.draw(viewPos, cameraVp, lightVp, light, textureId);
		head.draw(texture,cameraVp,head.getModel());
		upperBody.draw(texture, cameraVp, upperBody.getModel());
		lowerBody.draw(texture, cameraVp, lowerBody.getModel());
		tail.draw(texture, cameraVp, tail.getModel());
	}

	bool hasReached(glm::vec3 playerPos)
	{
		return  glm::distance(playerPos, pos) < 0.5;
	}

};
