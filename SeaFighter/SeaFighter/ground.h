#pragma once
#include <math.h>

#include "quad.h"


extern GLuint getTexture(std::string textureName, GLuint,GLuint);

class Ground : public Quad
{
private:
	GLuint texture;

public:
	Ground(glm::vec3 pos, glm::vec3 size) : Quad()
	{
		this->pos = pos;
		model = glm::translate(model, pos);
		model = glm::scale(model, size);
		//model = glm::rotate(model, rad, glm::vec3(0, 0, isPositive ? 1 : -1));

		texture = getTexture("Textures/sand.jpg",GL_REPEAT, GL_REPEAT);
	}

	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light, GLuint textureId)
	{
		Quad::draw(texture, cameraVp, model);
	}

};