#pragma once
#include "game_object.h"

class Sea: public GameObject{
private:
	std::vector<unsigned int> indices;
	float startX, startY;
	GLuint elementbuffer;

	void updateDataGPU() 
	{
		GameObject::updateDataGPU();

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
public:

	Sea(glm::vec3 position, std::string vShader = "def_vshader.glslx", std::string fShader = "def_fshader.glslx") 
		: GameObject::GameObject(position,vShader,fShader)
	{

	}

	void setSize(float sizeW, float sizeH,int nW, int nH)
	{
		vertices.clear();
		startX = pos[0]- sizeW / 2;
		startY = pos[1] - sizeH / 2;
		float tWidth = sizeW / (nW-1);
		float tHeight = sizeH / (nH-1);
		for (int i = 0; i < nH; i++) 
		{
			for (int j = 0; j < nW; j++)
			{
				vertices.push_back(Vertex(glm::vec3(startX + j * tWidth, startY + i * tHeight, 0.),
											glm::vec3(0.,0.,1)));
			}
		}
		for (int i = 0; i < nH - 1; i++)
		{
			for (int j = 0; j < nW - 1; j++)
			{
				indices.push_back(j + i * nW + 0);
				indices.push_back(j + i * nW + 1);
				indices.push_back(j + (i + 1) * nW);
				indices.push_back(j + i * nW + 1);
				indices.push_back(j + (i + 1) * nW);
				indices.push_back(j + (i + 1) * nW + 1);
			}
		}
		updateDataGPU();
	}

	void draw(glm::mat4 mvp, glm::vec3 viewPos, float time)
	{
		//maybe first check whether it should be drawn 
		shader();

		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform3fv(2, 1, glm::value_ptr(viewPos));
		std::cout << glfwGetTime()<<std::endl;
		glUniform1f(3, static_cast<float>(glfwGetTime()));
		glUniform1f(4, startX);
		glUniform1f(5, startY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// Bind the texture to slot 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT , (void*)0);


	}
};