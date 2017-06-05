#pragma once

#include "gl_controller.h"

#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "player.h"
#include "scene.h"
#include "utils.h"



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


class Game
{
private:
	struct PlayerKeyboardStates {
		bool rotateLeft;
		bool rotateRight;
		bool isAccelerating;
		bool isDecelerating;

		PlayerKeyboardStates() : rotateLeft(false), rotateRight(false),
			isAccelerating(false), isDecelerating(false)
		{}
	};

	Player player;
	PlayerKeyboardStates p_keyboardStates;

	Scene scene;
	Sea sea;
	std::vector<Island> islands;

	glm::vec3 lightPos;
	glm::vec3 viewPos;
	glm::vec3 cameraOffset;

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 inverseInitialMvp;

	bool isLightMoving = false;
	int width, height;


	void addAllObjectsToScene()
	{
		scene.addPlayer(&player, view * proj);
		//scene.addChild(&sea);
		//for (auto & island : islands)
		//	scene.addChild(&island);
	}

	std::function<void(int, double, double)> mouseClickFunction = [](int button, double xpos, double ypos) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			std::cout << xpos << " " << ypos;
		}
	};

	std::function<void(double, double)> mouseTraceFunction = [&](double xPos, double yPos) {
		glm::vec4 worldPos = getWorldCoordinate(inverseInitialMvp, xPos, yPos, width, height);
		player.rotateGunTo(worldPos);
	};

	std::function<void(int, bool)> keyFunction = [&](int button, bool isPressed) {
		switch (button)
		{
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			p_keyboardStates.rotateLeft = isPressed;
			break;
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			p_keyboardStates.rotateRight = isPressed;
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			p_keyboardStates.isAccelerating = isPressed ? true : false;
			break;
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			p_keyboardStates.isDecelerating = isPressed ? true : false;
			break;
		case GLFW_KEY_SPACE:
			if (isPressed)
				isLightMoving = !isLightMoving;
			break;
		}
	};

	void updatePlayerState()
	{
		if (p_keyboardStates.isAccelerating)
			player.accelerate();
		if (p_keyboardStates.isDecelerating)
			player.decelerate();
		if (p_keyboardStates.rotateLeft)
			player.rotate(-0.01);
		if (p_keyboardStates.rotateRight)
			player.rotate(0.01);
	}


	void updateCamera()
	{
		viewPos = glm::rotate(cameraOffset, (glm::mediump_float)player.getRotation(), glm::vec3(0, 0, -1)) + player.getPosition();
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
	}
public:

	Game(int width, int height) :
		sea(glm::vec3(0, 1.5, 0), "sea_vs.glslx", "sea_fs.glslx"),
		scene(width, height),
		player(glm::vec3(0, 0, 0)),
		width(width),
		height(height)
	{
		sea.setSize(20, 20, 500, 500);

		IslandFactory factory(9, 9, 0.1, 7, 3);
		islands = factory.getIslands(10);


		player.loadModel("Objects/boat_new.obj", "Objects/gun_new.obj");


		lightPos = glm::vec3(-5.f, 10.0f, 3.f);

		cameraOffset = glm::vec3(.0f, -2.0f, 2.05f);
		viewPos = cameraOffset;
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
		proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 30.0f);

		inverseInitialMvp = glm::inverse(proj * view);

		player.setModel(glm::scale(glm::mat4(), glm::vec3(0.9, 0.9, 0.9)));

		addAllObjectsToScene();

		GLController::getInstance().setKeyPressedFunction(keyFunction);
		GLController::getInstance().setMouseTraceFunction(mouseTraceFunction);
		GLController::getInstance().setMousePressFunction(mouseClickFunction);
	}


	void gameLoop(int frame)
	{
		updatePlayerState();
		player.updateSpeed();
		if (frame == 0 || player.updatePosition())
		{
			updateCamera();
		}

		scene.renderScene(viewPos, proj * view);
	}

	void updateLight()
	{
		lightPos = glm::rotateY(lightPos, (glm::mediump_float)1 / 1000);
		//std::cout << light[0] << " " << light[2] << std::endl;
		scene.setLight(lightPos);
	}

	bool shouldLightMove()
	{
		return isLightMoving;
	}
};
