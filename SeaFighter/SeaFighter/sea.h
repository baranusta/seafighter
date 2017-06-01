#pragma once
#include "game_object.h"
#include "height_field.h"

class Sea: public GameObject, public HeightField
{
private:
	std::vector<unsigned int> indices;
	float startX, startY, gridWidth, gridHeight;
	GLuint elementbuffer;

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
		gridWidth = sizeW / (nW-1);
		gridHeight = sizeH / (nH-1);
		Surface surface = generateSurface(startX, startY, gridWidth, gridHeight,nW,nH);
		indices = surface.indices;
		vertices = surface.vertices;
		surface.clean();

		updateDataGPU();
	}

	void draw(glm::mat4 mvp, glm::vec3 viewPos,glm::vec3 light)
	{
		//maybe first check whether it should be drawn 
		glBindVertexArray(vao);
		shader();
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

		//light
		glUniform3fv(1, 1, glm::value_ptr(light));
		glUniform3fv(2, 1, glm::value_ptr(viewPos));

		//std::cout << glfwGetTime()<<std::endl;
		glUniform1f(3, static_cast<float>(glfwGetTime()));
		glUniform1f(4, gridWidth);
		glUniform1f(5, gridHeight);
		//glUniform1f(4, startX);
		//glUniform1f(5, startY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		// Bind the texture to slot 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT , (void*)0);
		glBindVertexArray(0);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error sea: " << err << gluErrorString(err) << std::endl;
		}
	}
};