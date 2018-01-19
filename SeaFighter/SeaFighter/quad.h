#pragma once

#include "game_object.h"

class Quad : public GameObject
{
public:
	Quad() :GameObject(glm::vec3(), "quad_vs.glslx", "quad_fs.glslx")
	{
		vertices = {
			Vertex(glm::vec3(-1,-1,0)),
			Vertex(glm::vec3(1,-1,0)),
			Vertex(glm::vec3(1,1,0)),
			Vertex(glm::vec3(1,1,0)),
			Vertex(glm::vec3(-1,1,0)),
			Vertex(glm::vec3(-1,-1,0))
		};
		updateDataGPU();
	}

	Quad(glm::vec3 pos) :GameObject(pos, "quad_vs.glslx", "quad_fs.glslx")
	{
		vertices = {
			Vertex(glm::vec3(-1,-1,0)),
			Vertex(glm::vec3(1,-1,0)),
			Vertex(glm::vec3(1,1,0)),
			Vertex(glm::vec3(1,1,0)),
			Vertex(glm::vec3(-1,1,0)),
			Vertex(glm::vec3(-1,-1,0))
		};
		updateDataGPU();
	}

	virtual void draw()
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		printError("pfff1");

		shaderDraw();
		printError("pfff21");
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		printError("pfff3");
	}

	void draw(GLuint texture, glm::mat4 cameraVp, glm::mat4 model)
	{
		printError("pfff0");
		glBindVertexArray(vao);
		printError("pfff1");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		printError("pfff2");
		glBindTexture(GL_TEXTURE_2D, texture);
		printError("pfff3");

		shaderDraw();

		printError("pfff5");
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(cameraVp));
		printError("pfff6");
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(model));
		printError("pfff7");

		printError("pfff8");
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		printError("pfff9");
		glBindTexture(GL_TEXTURE_2D, 0);
		printError("pfff10");
	}

};