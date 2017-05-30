#include "gl_controller.h"
#include "game_object.h"
#include "sea.h"



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
			std::cout << xpos << " " << ypos;
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

	if (GLController::getInstance().initialize(width, height, "Sea Fighter"))
	{
		GLenum err;
		setMouseFunctions();
		setKeyFunctions();
		//GameObject boat(glm::vec3(0,0,0));
		//boat.loadModel("Objects/DavidHeadCleanMax.obj");
		Sea see(glm::vec3(0, 0, 0),"sea_vs.glslx");
		see.setSize(2, 2, 50, 50);
		glm::vec3 viewPos = glm::vec3(.0f, -2.0f, 2.0f);
		glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 10.0f);
		glm::mat4 model = glm::mat4();
		glm::mat4 mvp = proj * view * model;



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

			}
		});


		GLController::getInstance().startGame([&]() {
			//Game code here
			//boat.draw(mvp, viewPos, glfwGetTime());
			/*glm::mat4 speed = glm::mat4();
			speed[0][0] = 1.1;
			model = model * speed;
			mvp = proj * view * model;*/
			see.draw(mvp, viewPos, glfwGetTime());
		});
	}
	GLController::getInstance().stop();
}