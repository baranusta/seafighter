#pragma once

#include "shader_controller.h"
#include "tiny_obj_loader.h"
#include "common.h"
#include "gl_controller.h"




class GameObject
{

protected:
	struct Color {
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specConst;

	};

	Color color;
	glm::vec3 pos;
	std::vector<Vertex> vertices;
	ShaderController shaderDraw;
	ShaderController shaderShadow;

	bool isVisible;

	GLuint vao;
	GLuint vbo;
	glm::vec3 bottomLeft;
	glm::vec3 bottomRight;
	glm::vec3 topLeft;
	glm::vec3 topRight;

	glm::mat4 model;

	virtual void updateDataGPU() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindVertexArray(vao);

		// The position vectors should be retrieved from the specified Vertex Buffer Object with given offset and stride
		// Stride is the distance in bytes between vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// The normals should be retrieved from the same Vertex Buffer Object (glBindBuffer is optional)
		// The offset is different and the data should go to input 1 instead of 0
		if (vertices.size() && vertices[0].hasNormal)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
			glEnableVertexAttribArray(1);
		}


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

public:
	GameObject()
	{
		color.diffuse = glm::vec4(0.4, .25, .16, 1.0);
		color.ambient = glm::vec4(0.3, .2, .1, 1.0);
		color.specConst = glm::vec4(0, 0, 0, 1.0);
	}

	GameObject(glm::vec3 position, std::string vShader = "def_vshader.glslx", std::string fShader= "def_fshader.glslx") 
		: pos(position), shaderDraw(vShader,fShader), shaderShadow(vShader, "shadow.glslx")
	{
		color.diffuse = glm::vec4(0.4, .25, .16, 1.0);
		color.ambient = glm::vec4(0.3, .2, .1, 1.0);
		color.specConst = glm::vec4(0, 0, 0, 1.0);

		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);


		updateDataGPU();
	}

	std::vector<Vertex> getVertices()
	{
		return vertices;
	}

	virtual void setVertices(std::vector<Vertex> vertices)
	{
		this->vertices = vertices;
		updateDataGPU();
	}

	void setColor(Color c)
	{
		color = c;
	}

	virtual ~GameObject()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	glm::vec3 getPosition()
	{
		return pos;
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
				if (attrib.normals.size() > 0) {
				Vertex vertex(
					glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
								attrib.vertices[3 * index.vertex_index + 1],
								attrib.vertices[3 * index.vertex_index + 2]),
					glm::vec3(attrib.normals[3 * index.normal_index + 0],
								attrib.normals[3 * index.normal_index + 1],
								attrib.normals[3 * index.normal_index + 2]));
				vertices.push_back(vertex);
				}
				else
				{

					Vertex vertex(
						glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]),
						glm::vec3(0,0,0));
					vertices.push_back(vertex);
				}

				//+++++++++++++++++++++++++++++++++++
				//+++++++++++++++++++++++++++++++++++
				// If we need more info like texture
				// we need to add it here
				//+++++++++++++++++++++++++++++++++++
				//+++++++++++++++++++++++++++++++++++
			}
		}
			BBox bbox;
			for (auto vertex : getVertices())
			{
				glm::vec3& v_pos = vertex.Position;
				bbox.xMin = std::min(bbox.xMin, v_pos[0]);
				bbox.xMax = std::max(bbox.xMax, v_pos[0]);
				bbox.yMin = std::min(bbox.yMin, v_pos[1]);
				bbox.yMax = std::max(bbox.yMax, v_pos[1]);
			}
			bottomLeft = glm::vec3(bbox.xMin, bbox.yMin, 0);
			bottomRight = glm::vec3(bbox.xMax, bbox.yMin, 0);
			topLeft = glm::vec3(bbox.xMin, bbox.yMax, 0);
			topRight = glm::vec3(bbox.xMax, bbox.yMax, 0);

		updateDataGPU();
		return true;
	}

	virtual void setModel(glm::mat4 model)
	{
		this->model = model;
	}

	glm::mat4 getModel()
	{
		return model;
	}

	std::vector<glm::vec3> getPoints()
	{
		return { model * glm::vec4(bottomLeft,1.),
			model * glm::vec4(bottomRight,1.),
			model * glm::vec4(topLeft,1.),
			model * glm::vec4(topRight,1.) };
	}

	virtual void setPosition(glm::vec3 position)
	{
		this->pos = position;
	}

	void rotate(float angle, glm::vec3 axis)
	{
		model = glm::rotate(model, angle, axis);
	}

	void setVisibility(bool visibility)
	{
		isVisible = visibility;
	}

	void passBasicsToGPU(ShaderController& shader,glm::mat4 vp, glm::vec3 viewPos, glm::vec3 light, glm::mat4 lightVp, GLuint textureId)
	{
		glBindVertexArray(vao);
		printError("yossss5st");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		printError("yossss4st");
		shader();

		printError("asdasdff");
		glUniform1i(9, 0);
		printError("pfff");
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
		glUniformMatrix4fv(8, 1, GL_FALSE, glm::value_ptr(lightVp));
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(model));

		//light
		glUniform3fv(1, 1, glm::value_ptr(light));
		printError("pfff1");
		// Set view position
		glUniform3fv(2, 1, glm::value_ptr(viewPos));
		printError("pfff2");
		// Expose current time in shader uniform
		glUniform1f(3, static_cast<float>(glfwGetTime()));
		printError("pfff3");
		glUniform4fv(4, 1, glm::value_ptr(color.ambient));
		printError("pfff4");
		glUniform4fv(5, 1, glm::value_ptr(color.diffuse));
		printError("pfff5");
		glUniform4fv(6, 1, glm::value_ptr(color.specConst));
		printError("pfff6");

	}

	virtual void draw(glm::vec3 viewPos,glm::mat4 cameraVp, glm::mat4 lightVp, glm::vec3 light,  GLuint textureId)
	{
		if (!isVisible)
			return;

		passBasicsToGPU(shaderDraw, cameraVp, viewPos, light, lightVp, textureId);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);
		
		//// Execute draw command
		printError("qqqqqqqqq");
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		printError("wwwwwwwwwwf");

		printError("Game object draw end");
	}

	virtual void renderShadowMap(glm::mat4 lightVp)
	{
		if (!isVisible)
			return;

		printError("yo5");
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		shaderShadow();

		glUniform1i(9, 1);
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightVp));
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(model));

		printError("yo2");
		glUniform1f(3, static_cast<float>(glfwGetTime()));

		printError("yo1");
		//// Execute draw command
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		printError("ya");

		printError("Game object shadow end");
		glBindVertexArray(0);
	}
};
