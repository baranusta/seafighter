#include "gl_controller.h"
#include "game_object.h"



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#include <iostream>
#include <vector>

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
	GLController::getInstance().setKeyPressedFunction([](int button) {
		switch (button)
		{
		case GLFW_KEY_LEFT:
			std::cout << "turn left";
			break;
		case GLFW_KEY_RIGHT:
			std::cout << "turn right";
			break;

		}
	});
}

int main()
{
	int width = 400;
	int height = 500;

	if (GLController::getInstance().initialize(width, height, "Sea Fighter"))
	{
		GLenum err;
		setMouseFunctions();
		setKeyFunctions();
		GameObject boat;
		glm::vec3 viewPos = glm::vec3(-0.8f, 0.7f, -0.5f);

		glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 10.0f);
		glm::mat4 model = glm::mat4();
		glm::mat4 mvp = proj * view * model;
		GLController::getInstance().startGame([&]() {
			//Game code here
			boat.draw(mvp, viewPos, glfwGetTime());

		});
	}
	GLController::getInstance().stop();
}