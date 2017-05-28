#pragma once

#include "shader_controller.h"
#include "tiny_obj_loader.h"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"



struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	Vertex(glm::vec3 pos, glm::vec3 normal) :Position(pos), Normal(normal) {}
};

class GameObject
{
private:
	glm::vec3 pos;
	std::vector<Vertex> vertices;
	ShaderController shader;

	GLuint vao;
	GLuint vbo;


public:
	GameObject(): shader("def_vshader.glslx", "def_fshader.glslx")
	{
		vertices = {Vertex(glm::vec3(0,0,0),glm::vec3(0,0,1)),
					Vertex(glm::vec3(-1,0,0),glm::vec3(0,0,1)),
					Vertex(glm::vec3(0,1,0),glm::vec3(0,0,1)) };
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// Bind vertex data to shader inputs using their index (location)
		// These bindings are stored in the Vertex Array Object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// The position vectors should be retrieved from the specified Vertex Buffer Object with given offset and stride
		// Stride is the distance in bytes between vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glEnableVertexAttribArray(0);

		// The normals should be retrieved from the same Vertex Buffer Object (glBindBuffer is optional)
		// The offset is different and the data should go to input 1 instead of 0
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glEnableVertexAttribArray(1);
	}

	//Returns false if given model is not found
	bool loadModel(std::string object_name)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, object_name.c_str() )) {//"dragon.obj" //Older name of the object
			return false;
		}

		//
		// Read triangle vertices from OBJ file
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex(
					glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
								attrib.vertices[3 * index.vertex_index + 1],
								attrib.vertices[3 * index.vertex_index + 2]),
					glm::vec3(attrib.normals[3 * index.normal_index + 0],
								attrib.normals[3 * index.normal_index + 1],
								attrib.normals[3 * index.normal_index + 2]));
				vertices.push_back(vertex);

				//+++++++++++++++++++++++++++++++++++
				//+++++++++++++++++++++++++++++++++++
				// If we need more info like texture
				// we need to add it here
				//+++++++++++++++++++++++++++++++++++
				//+++++++++++++++++++++++++++++++++++
			}
		}
		return true;
	}

	virtual void draw(glm::mat4 mvp, glm::vec3 viewPos, float time)
	{
		//maybe first check whether it should be drawn 
		shader();

		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

		// Set view position
		glUniform3fv(2, 1, glm::value_ptr(viewPos));

		// Expose current time in shader uniform
		glUniform1f(3, static_cast<float>(glfwGetTime()));

		// Bind vertex data
		glBindVertexArray(vao);
		// Bind the texture to slot 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);
		
		//// Execute draw command
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	}
};
