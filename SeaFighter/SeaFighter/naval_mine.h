#pragma once
#include "shader_controller.h"
#include "tiny_obj_loader.h"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include <stdlib.h>
#include "common.h"

// Library for loading an image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texToon;
GLuint textExplotion;

//class Mine : public GameObject
//{
//
//public:
//
//	Mine() : GameObject(glm::vec3())  {
//
//	}
//
//	Mine(glm::vec3 position) : pos(position), shader("navalmine_vshader.glslx", "navalmine_fshader.glslx")
//			//GameObject() : shader("vert.glslx", "fs.glslx")
//
//	{
//
//		vertices = { Vertex(glm::vec3(0,0,0),glm::vec3(0,0,1)),
//			Vertex(glm::vec3(-1,0,0),glm::vec3(0,0,1)),
//			Vertex(glm::vec3(0,1,0),glm::vec3(0,0,1)) };
//
//	}
//
//	void buildMine(double gridSize)
//	{
//		pos = glm::vec3(m_startX, m_startY, 0);
//
//		updateDataGPU();
//	}
//
//	//Returns false if given model is not found
//	bool loadModel()//std::string object_name
//	{
//
//		vertices.clear();
//		tinyobj::attrib_t attrib;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//		std::string err;
//
//		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "watermine7.obj")) {// //Older name of the object object_name.c_str()
//			std::cout << "There is no file here" << std::endl;
//			return false;
//		}
//		std::cout << "Object readed" << std::endl;
//		//std::vector<Vertex> vertices;
//
//
//		// Read triangle vertices from OBJ file
//		for (const auto& shape : shapes) {
//			for (const auto& index : shape.mesh.indices) {
//				Vertex vertex(
//					glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
//						attrib.vertices[3 * index.vertex_index + 1],
//						attrib.vertices[3 * index.vertex_index + 2]),
//
//					glm::vec3(attrib.normals[3 * index.normal_index + 0],
//						attrib.normals[3 * index.normal_index + 1],
//						attrib.normals[3 * index.normal_index + 2]));
//				vertices.push_back(vertex);
//
//				//+++++++++++++++++++++++++++++++++++
//				//+++++++++++++++++++++++++++++++++++
//				// If we need more info like texture
//				// we need to add it here
//				//+++++++++++++++++++++++++++++++++++
//				//+++++++++++++++++++++++++++++++++++
//			}
//		}
//
//		glGenBuffers(1, &vbo);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//		// Bind vertex data to shader inputs using their index (location)
//		// These bindings are stored in the Vertex Array Object
//		glGenVertexArrays(1, &vao);
//		glBindVertexArray(vao);
//
//		// The position vectors should be retrieved from the specified Vertex Buffer Object with given offset and stride
//		// Stride is the distance in bytes between vertices
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
//		glEnableVertexAttribArray(0);
//
//		// The normals should be retrieved from the same Vertex Buffer Object (glBindBuffer is optional)
//		// The offset is different and the data should go to input 1 instead of 0
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
//		glEnableVertexAttribArray(1);
//
//
//		//Set a texture to the 
//
//
//		// Load image
//		int width, height, channels;
//		stbi_uc* pixels = stbi_load("images/universe.png", &width, &height, &channels, 3);
//		//stbi_uc* pixels = stbi_load("boat_1.png", &width, &height, &channels, 3);
//		//stbi_uc* pixels = stbi_load("boat_n.tga", &width, &height, &channels, 3);
//
//		// Create Texture
//		//GLuint texToon; Was declared locally
//		glGenTextures(1, &texToon);
//		glBindTexture(GL_TEXTURE_2D, texToon);
//
//		// Upload pixels into texture
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//
//		// Set behaviour for when texture coordinates are outside the [0, 1] range
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//		// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// Enable depth testing
//		glEnable(GL_DEPTH_TEST);
//
//
//		return true;
//
//	}
//
//	bool updateTexture(std::string texture_name) {
//		// Load image
//		int width, height, channels;
//		stbi_uc* pixels = stbi_load(("images/" + texture_name + ".png").c_str(), &width, &height, &channels, 3);
//
//		// Create Texture
//		//GLuint texToon; Was declared locally
//		glGenTextures(1, &texToon);
//		glBindTexture(GL_TEXTURE_2D, texToon);
//
//		// Upload pixels into texture
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//
//		// Set behaviour for when texture coordinates are outside the [0, 1] range
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//		// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// Enable depth testing
//		glEnable(GL_DEPTH_TEST);
//
//		return true;
//
//	}
//
//	virtual void draw(glm::mat4& mvp, glm::vec3 viewPos, float time, glm::vec3 newPos, float size)
//	{
//		//maybe first check whether it should be drawn 
//		shader();
//		glm::mat4 speed = glm::mat4();
//		speed[0][0] = 0.1;
//		//mvp = mvp + speed;
//		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
//
//		// Set view position
//		//glUniform3fv(2, 1, glm::value_ptr(viewPos));
//		glUniform3fv(2, 1, glm::value_ptr(viewPos));
//
//		// Expose current time in shader uniform
//		glUniform1f(3, static_cast<float>(glfwGetTime()));
//
//		//New movement and new Matrix
//		glUniform3fv(6, 1, glm::value_ptr(newPos));
//		glUniform1f(7, size);
//
//
//		// Bind vertex data
//		glBindVertexArray(vao);
//		// Bind the texture to slot 0
//		//glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_2D, texToon);
//
//		//IF there is a texture available
//		if (true) {
//			// Bind the texture to slot 0
//			glActiveTexture(GL_TEXTURE0);
//			glBindTexture(GL_TEXTURE_2D, texToon);
//			glUniform1i(2, 0);
//		}
//
//		//// Execute draw command
//		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
//
//	}
//
//protected:
//	//Positions in real world
//	float m_startX, m_startY;
//	//This should inheritate frome Game Object
//
//	struct Color {
//		glm::vec4 ambient;
//		glm::vec4 diffuse;
//		glm::vec4 specConst;
//
//	};
//
//private:
//	Color color;
//	glm::vec3 pos;
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//	//GLuint elementbuffer;
//
//	ShaderController shader;
//	GLuint vao;
//	GLuint vbo;
//
//	virtual void updateDataGPU() {
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//		glBindVertexArray(vao);
//
//		// The position vectors should be retrieved from the specified Vertex Buffer Object with given offset and stride
//		// Stride is the distance in bytes between vertices
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//		// The normals should be retrieved from the same Vertex Buffer Object (glBindBuffer is optional)
//		// The offset is different and the data should go to input 1 instead of 0
//		if (vertices.size() && vertices[0].hasNormal)
//		{
//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
//			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
//			glEnableVertexAttribArray(1);
//		}
//
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//	}
//};


class Mine
{

public:

	Mine() {

	}

	Mine(glm::vec3 position) : pos(position), shader("navalmine_vshader.glslx", "navalmine_fshader.glslx")
				//GameObject() : shader("vert.glslx", "fs.glslx")

	{	
		
		vertices = { Vertex(glm::vec3(0,0,0),glm::vec3(0,0,1)),
			Vertex(glm::vec3(-1,0,0),glm::vec3(0,0,1)),
			Vertex(glm::vec3(0,1,0),glm::vec3(0,0,1)) };
		
	}

	void buildMine(double gridSize)
	{
		pos = glm::vec3(m_startX , m_startY,-0.2);

		updateDataGPU();
	}

	//Returns false if given model is not found
	bool loadModel()//std::string object_name
	{

		vertices.clear();
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "watermine7.obj")) {// //Older name of the object object_name.c_str()
			std::cout << "There is no file here" << std::endl;
			return false;
		}
		std::cout << "Object readed" << std::endl;
		//std::vector<Vertex> vertices;


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


		//Set a texture to the 


		// Load image
		int width, height, channels;
		stbi_uc* pixels = stbi_load("images/toon_map.png", &width, &height, &channels, 3);
		//stbi_uc* pixels = stbi_load("boat_1.png", &width, &height, &channels, 3);
		//stbi_uc* pixels = stbi_load("boat_n.tga", &width, &height, &channels, 3);

		// Create Texture
		//GLuint texToon; Was declared locally
		glGenTextures(1, &texToon);
		glBindTexture(GL_TEXTURE_2D, texToon);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// Set behaviour for when texture coordinates are outside the [0, 1] range
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);


		return true;

	}

	bool updateTexture(std::string texture_name ) {
		// Load image
		int width, height, channels;
		stbi_uc* pixels = stbi_load(("images/" + texture_name +".png").c_str(), &width, &height, &channels, 3);

		// Create Texture
		//GLuint texToon; Was declared locally
		glGenTextures(1, &texToon);
		glBindTexture(GL_TEXTURE_2D, texToon);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// Set behaviour for when texture coordinates are outside the [0, 1] range
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		return true;

	}

	virtual void draw(glm::mat4& mvp, glm::vec3 viewPos, float time, glm::vec3 newPos, float size)
	{
		//maybe first check whether it should be drawn 
		shader();
		glm::mat4 speed = glm::mat4();
		speed[0][0] = 0.1;
		//mvp = mvp + speed;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

		// Set view position
		//glUniform3fv(2, 1, glm::value_ptr(viewPos));
		glUniform3fv(2, 1, glm::value_ptr(viewPos));

		// Expose current time in shader uniform
		glUniform1f(3, static_cast<float>(glfwGetTime()));

		//New movement and new Matrix
		glUniform3fv(6, 1, glm::value_ptr(newPos));
		glUniform1f(7, size);


		// Bind vertex data
		glBindVertexArray(vao);
		// Bind the texture to slot 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texToon);

		//IF there is a texture available
		if (true) {
			// Bind the texture to slot 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texToon);
			glUniform1i(2, 0);
		}

		//// Execute draw command
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	}

protected:
		//Positions in real world
		float m_startX, m_startY;
		//This should inheritate frome Game Object

		struct Color {
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specConst;

		};

private:
	Color color;
	glm::vec3 pos;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//GLuint elementbuffer;

	ShaderController shader;
	GLuint vao;
	GLuint vbo;

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
};











