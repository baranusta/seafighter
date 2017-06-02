#include "gl_controller.h"
#include "game_object.h"
#include "sea.h"
#include "island_factory.h"



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
		try {

			Sea see(glm::vec3(0, 1.5, 0), "sea_vs.glslx", "sea_fs.glslx");
			see.setSize(6, 7, 150, 175);

			IslandFactory factory(9, 9, 0.1, 7, 3);
			auto islands = factory.getIslands(10);


			glm::vec3 light = glm::vec3(-5.0f, 10.0f, 5.0f);
			bool isLightMoving = false;

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
				case GLFW_KEY_A:
					isLightMoving = !isLightMoving;
					break;
				}
			});

			float frame = 0;
			GLController::getInstance().startGame([&]() {

				if (isLightMoving)
					light = glm::rotateY(light, (glm::mediump_float)1 / 10000);
				if(light[2]>0)
					std::cout << light[0] << " " << light[2] << std::endl;

				see.draw(mvp, viewPos, light);
				for (auto & island : islands)
					island.draw(mvp, viewPos, light);
				frame++;
			});

		}
		catch (const char* c)
		{
			std::cout << c << std::endl;
		}
		catch (...) {
			std::cerr << "Caught unknown exception." << std::endl;
		}
	}
	GLController::getInstance().stop();
}