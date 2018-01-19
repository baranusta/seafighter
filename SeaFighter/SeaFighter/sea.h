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

	void draw(glm::vec3 viewPos, glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light, GLuint textureId)
	{
		printError("1 ");
		//maybe first check whether it should be drawn 
		glBindVertexArray(vao);
		shaderDraw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glUniform1i(9, 0);
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(cameraVp));
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(8, 1, GL_FALSE, glm::value_ptr(lightVp));

		//light
		glUniform3fv(1, 1, glm::value_ptr(light));
		glUniform3fv(2, 1, glm::value_ptr(viewPos));

		glUniform1f(3, static_cast<float>(glfwGetTime()));
		glUniform1f(4, gridWidth);
		glUniform1f(5, gridHeight);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT , (void*)0);

		printError("6 "); 
		glBindVertexArray(0);

		printError("OpenGL error sea: ");
	}

	void renderShadowMap(glm::mat4 lightVp)
	{
		//maybe first check whether it should be drawn 
		printError("A");
		glBindVertexArray(vao);
		shaderDraw();
		printError("B");
		glUniform1i(9, 1);
		printError("C");
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightVp));
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(model));
		printError("D");

		glUniform1f(3, static_cast<float>(glfwGetTime()));
		printError("E");
		glUniform1f(4, gridWidth);
		glUniform1f(5, gridHeight);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		printError("F");

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		printError("D");
		glBindVertexArray(0);

		printError("yo");
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error sea: " << err << gluErrorString(err) << std::endl;
		}
	}
};