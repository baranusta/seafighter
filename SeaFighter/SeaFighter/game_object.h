#pragma once

#include "shader_controller.h"
#include "tiny_obj_loader.h"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
};

class GameObject
{
private:
	glm::vec3 pos;
	std::vector<Vertex> vertices;
	ShaderController shader;

public:
	GameObject(): shader("def_fshader.glslx","def_vshader.glslx")
	{

	}

	//Returns false if given model is not found
	bool loadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "dragon.obj")) {
			return false;
		}


		// Read triangle vertices from OBJ file
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex = {};

				// Retrieve coordinates for vertex by index
				vertex.Position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				// Retrieve components of normal by index
				vertex.Normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};

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

	virtual void draw()
	{
		//maybe first check whether it should be drawn 
		shader();

	}


};
