#include "gl_controller.h"
#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "naval_mine.h"



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#include <iostream>
#include <vector>
#include "glm/glm/gtc/matrix_transform.hpp"



void setMouseFunctions()
{
	GLController::getInstance().setMousePressFunction([](int button, double xpos, double ypos) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			//std::cout << xpos << " " << ypos;
		}
	});
}

void setKeyFunctions()
{
}

int main()
{
	
	int width = 600;
	int height = 500;
	glm::vec3  newPos(0,0,0);
	float newEnemy;

	int movementX, movementY, movementZ;
	movementX, movementY, movementZ = 0.0f;

	if (GLController::getInstance().initialize(width, height, "Sea Fighter"))
	{
		GLenum err;
		setMouseFunctions();
		setKeyFunctions();
		//GameObject boat(glm::vec3(0,0,0));		
		//float loaded = boat.loadModel("bunny.obj");
		Sea see(glm::vec3(0, 0, 0), "sea_vs.glslx", "sea_fs.glslx");
		see.setSize(2, 2, 50, 50);
		
		//Mines creation
		Mine enemy(glm::vec3(0,0,0));
		enemy.loadModel();

		//New enemy
		Mine newEnemy(glm::vec3(1,0,0));
		newEnemy.loadModel();
		glm::vec3 my3DVector;
		
		//Change size of the mines and after colliding with them
		float x, y, z, w;
		x = y = z = 2;
		w = 1;
		float size = 0.00001;
		
		glm::mat4 myRotationMatrix;
		glm::mat4 myScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		glm::vec4 myVector(x,y,z,w);
		glm::vec4 transformedVector = myScalingMatrix * myVector;
		std::cout << "Something" << std::endl;
	
		//Island Creation
		IslandFactory factory(2, 2, 0.1);
		auto islands = factory.getIslands(1);

		//Lights
		glm::vec3 light = glm::vec3(-5.0f, 10.0f, 5.0f);
		bool isLightMoving = false;
		
		glm::vec3 viewPos = glm::vec3(.0f, -2.0f, 2.0f);
		glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 10.0f);
		glm::mat4 model = glm::mat4();
		glm::mat4 mvp = proj * view * model;
		
		//enemy.lo

		GLController::getInstance().setKeyPressedFunction([&](int button) {
			
			switch (button)
			{
			case GLFW_KEY_LEFT:
				model = glm::rotate(model, (glm::mediump_float)0.05, glm::vec3(0, 0, -1));
				mvp = proj * view * model;
				break;
			case GLFW_KEY_RIGHT:
				model = glm::rotate(model, (glm::mediump_float)0.05, glm::vec3(0, 0, 1));
				mvp = proj * view * model;
				break;

			case GLFW_KEY_L:
				isLightMoving = !isLightMoving;
				break;

			//case GLFW_KEY_W:
			//	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1));
			//	mvp = proj * view * model;
			//	break;
			//case GLFW_KEY_S:
			//	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1));
			//	mvp = proj * view * model;
			//	break;

				//Create enemy
			case GLFW_KEY_E:
				//Mine* myNewEnemy = (Mine*)malloc(sizeof(Mine));
				//new(myNewEnemy) Mine(glm::vec3(0,0,0));
				//myNewEnemy->loadModel();
				//myNewEnemy->draw(mvp, viewPos, glfwGetTime(), glm::vec3(0,0,0));
				//newEnemy.loadModel();
				//newEnemy.draw(mvp, viewPos, glfwGetTime(), glm::vec3(0, 0, 0));
				size += 0.05;
				break;
			case GLFW_KEY_R:
				size -= 0.05;
				break;

			case GLFW_KEY_W:
				//newEnemyPosition = glm::rotate(newEnemyPosition, (glm::mediump_float) 0.05, glm::vec3(0,0,-1));
				//newPos = glm::translate(glm::mat4(1.0f), glm::vec3(1,0.0f,0.0f));
				//myRotationMatrix = glm::rotate(model, (glm::mediump_float)0.05, glm::vec3(0, 0, 1));
				movementX =  1;
				newPos = glm::vec3(0, 0.5f, 0);	
				break;
			case GLFW_KEY_S:
				movementY =  1;
				newPos = glm::vec3(0, -0.5f, 0);
				break;
			case GLFW_KEY_A:
				movementZ = 1;
				newPos = glm::vec3(-0.5f, 0, 0);
				//newMatrixPosition = glm::translate(glm::mat4(1), newPos);
				break;
			case GLFW_KEY_D:
				movementX ,movementY, movementZ = 0;
				newPos = glm::vec3(0.5f, 0, 0);
				//newMatrixPosition = glm::translate(glm::mat4(1), newPos);
				break;
			case GLFW_KEY_F:
				movementX, movementY, movementZ = 0;
				size = 0.0f;
				newPos = glm::vec3(0, 0, 0);
				//newMatrixPosition = glm::translate(glm::mat4(1), newPos);
				break;

			}


			
		});

		float frame = 0;
		GLController::getInstance().startGame([&]() {

			if (isLightMoving)
				light = glm::rotateY(light, (glm::mediump_float)1 / 10000);

			std::cout << light[0] << " " << light[2] << std::endl;
			//boat.draw(mvp, viewPos, light);
			//boat.drawUnitCube();
			//newPos = glm::vec3(0, 1, 0);
			
			enemy.draw(mvp, viewPos, glfwGetTime(), newPos, size);
			//newEnemy.draw(mvp, viewPos, glfwGetTime(), glm::vec3(0, 0, 0), myScalingMatrix);

			see.draw(mvp,viewPos,light);
		
			for (auto & island : islands)
				island.draw(mvp, viewPos, glfwGetTime());
			frame++;
		
			
		});
	}
	GLController::getInstance().stop();
}