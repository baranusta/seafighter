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
};