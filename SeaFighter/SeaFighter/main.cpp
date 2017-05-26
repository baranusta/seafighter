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
	if (GLController::getInstance().initialize(300, 400, "Sea Fighter"))
	{
		setMouseFunctions();
		setKeyFunctions();
		GLController::getInstance().startGame([]() {
			//Game code here
			GameObject boat;

			boat.draw();

		});
	}
	GLController::getInstance().stop();
}