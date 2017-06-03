#pragma once
#include "shader_controller.h"
#include "tiny_obj_loader.h"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include <stdlib.h>

// Library for loading an image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texToon;

/*
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	Vertex(glm::vec3 pos, glm::vec3 normal) :Position(pos), Normal(normal) {}
};
*/


//class NavalMine : public GameObject 
//{
//private:
//
//	//For new update GPU
//	std::vector<unsigned int> indices;
//	float startX, startY, gridWidth, gridHeight;
//	
//	//Change of all the mines
//	float changeX, changeY;
//	GLuint elementbuffer;
//
//	//Here is my old implementation
//	glm::vec3 pos;
//	std::vector<Vertex> vertices;
//	//ShaderController shader;
//	GLuint vao;
//	GLuint vbo;
//
//	void updateDataGPU2()
//	{
//		GameObject::updateDataGPU();
//
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glBindVertexArray(vao);
//		glGenBuffers(1, &elementbuffer);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//	}
//
//
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
//		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
//
//
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
//
//
//public:
//	
//	
//	NavalMine(glm::vec3 position, std::string vShader = "def_vshader.glslx", std::string fShader = "def_fshader.glslx")
//	    : GameObject::GameObject(position, vShader, fShader)
//		//: pos(position), shader("navalmine_vshader.glslx", "navalmine_fshader.glslx")
//	{
//		vertices = { Vertex(glm::vec3(0,0,0),glm::vec3(0,0,1)),
//			Vertex(glm::vec3(-1,0,0),glm::vec3(0,0,1)),
//			Vertex(glm::vec3(0,1,0),glm::vec3(0,0,1)) };
//	}
//	
//	void createTheMine(float startX, float startY) {
//
//		//bool response = loadModel("bunny.obj");
//		std::cout << "********Creating object********" << std::endl;
//		//draw();
//	}
//
//	void setSize(float sizeW, float sizeH, int nW, int nH)
//	{
//		vertices.clear();
//		startX = pos[0] - sizeW / 2;
//		startY = pos[1] - sizeH / 2;
//		gridWidth = sizeW / (nW - 1);
//		gridHeight = sizeH / (nH - 1);
//		//Surface surface = generateSurface(startX, startY, gridWidth, gridHeight, nW, nH);
//		//indices = surface.indices;
//		//vertices = surface.vertices;
//		//surface.clean();
//
//		updateDataGPU();
//	}
//
//	//Returns false if given model is not found
//	bool loadModelObject()//std::string object_name
//	{
//
//		vertices.clear();
//		tinyobj::attrib_t attrib;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//		std::string err;
//
//		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "dragon.obj")) {// //Older name of the object object_name.c_str()
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
//
//
//
//		//Set a texture and load image
//		int width, height, channels;
//		stbi_uc* pixels = stbi_load("toon_map.png", &width, &height, &channels, 3);
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
//	void draw(glm::mat4 mvp, glm::vec3 viewPos, glm::vec3 light, glm::vec3 newPos)
//	{
//		//maybe first check whether it should be drawn 
//		glBindVertexArray(vao);
//		shader();
//		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
//
//		//light
//		glUniform3fv(1, 1, glm::value_ptr(light));
//		glUniform3fv(2, 1, glm::value_ptr(viewPos));
//
//		//std::cout << glfwGetTime()<<std::endl;
//		glUniform1f(3, static_cast<float>(glfwGetTime()));
//		glUniform1f(4, gridWidth);
//		glUniform1f(5, gridHeight);
//		//glUniform1f(6, newPos);
//
//		
//		//glUniform1f(4, startX);
//		//glUniform1f(5, startY);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//		// Bind the texture to slot 0
//		//glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_2D, texToon);
//
//		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
//		glBindVertexArray(0);
//
//		GLenum err;
//		while ((err = glGetError()) != GL_NO_ERROR) {
//			std::cout << "OpenGL error sea: " << err << gluErrorString(err) << std::endl;
//		}
//	}
//
//};




/****************************************************************SECOND PART*********************************************/


class Mine
{
private:
	glm::vec3 pos;
	std::vector<Vertex> vertices;
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

public:


	Mine(glm::vec3 position) 
		: pos(position), shader("navalmine_vshader.glslx", "navalmine_fshader.glslx")
		
		//GameObject() : shader("vert.glslx", "fs.glslx")

	{
		vertices = { Vertex(glm::vec3(0,0,0),glm::vec3(0,0,1)),
			Vertex(glm::vec3(-1,0,0),glm::vec3(0,0,1)),
			Vertex(glm::vec3(0,1,0),glm::vec3(0,0,1)) };

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
		stbi_uc* pixels = stbi_load("toon_map.png", &width, &height, &channels, 3);
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
};











