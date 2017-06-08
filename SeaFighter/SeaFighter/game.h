#pragma once

#include "gl_controller.h"

#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "player.h"
#include "monster.h"
#include "scene.h"
#include "bullet.h"
#include "text.h"

#include <list>



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

extern glm::vec4 getWorldCoordinate(glm::mat4 matrix, int xPos, int yPos, int width, int height, float z = 0.94975);
float getOriginsZ(glm::mat4 matrix);

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

	Text score;
	Scene scene;
	Sea sea;
	Monster monster;
	bool isMonsterAlive;

	std::vector<Island> islands;
	std::list<Bullet*> bullets;

	glm::vec3 lightPos;
	glm::vec3 viewPos;
	glm::vec3 cameraOffset;

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 inverseInitialMvp;

	bool isLightMoving = false;
	int width, height;

	float originsZ;
	glm::vec3 visibleTopLeft;
	glm::vec3 visibleTopRight;
	glm::vec3 visibleBottomLeft;
	glm::vec3 visibleBottomRight;

	BBox gameArea;


	void addAllObjectsToScene()
	{
		//scene.addPlayer(&player, proj * view);
		//scene.addChildForRender(&sea);
		scene.addChildForRender(&monster);
		//for (auto & island : islands)
		//{
		//	scene.addChildForRender(&island);
		//	scene.addChildForShadow(&island);
		//}

	}

	std::function<void(int, double, double)> mouseClickFunction = [&](int button, double xpos, double ypos) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			glm::vec3 direction = player.getGunDirection();
			bullets.push_back(new Bullet(player.getPosition(), direction, islands));
		}
	};

	std::function<void(double, double)> mouseTraceFunction = [&](double xPos, double yPos) {
		glm::vec3 worldPos = getWorldCoordinate(inverseInitialMvp, xPos, yPos, width, height);
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
		case GLFW_KEY_Q:
			scene.changeRenderedCam();
				break;
		case GLFW_KEY_X:
			if (!isStarted)
			{
				monster.simplifyHead();
				isStarted = true; 
			}
			break;
		}
	};
	bool isStarted = false;
	void updatePlayerState()
	{
		if (p_keyboardStates.isAccelerating)
			player.accelerate();
		if (p_keyboardStates.isDecelerating)
			player.decelerate();
		if (p_keyboardStates.rotateLeft)
			player.rotate(-0.03);
		if (p_keyboardStates.rotateRight)
			player.rotate(0.03);
	}


	void updateCamera()
	{
		viewPos = glm::rotate(cameraOffset, (glm::mediump_float)player.getRotation(), glm::vec3(0, 0, -1)) + player.getPosition();
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
	}


	void updateVisibleAreas(glm::mat4 Vp)
	{
		visibleTopLeft = getWorldCoordinate(Vp, 0, 0, width, height, originsZ);
		visibleTopRight = getWorldCoordinate(Vp, width - 1,0 , width, height, originsZ);
		visibleBottomLeft = getWorldCoordinate(Vp, 0, height - 1, width, height, originsZ);
		visibleBottomRight = getWorldCoordinate(Vp, width - 1, height - 1, width, height, originsZ);
	}

public:

	Game(int width, int height) :
		gameArea(BBox(-10.,+10.,-10.,+10.)),
		score("Holstein.DDS"),
		sea(glm::vec3((gameArea.xMax - gameArea.xMin)/2, (gameArea.yMax - gameArea.yMin) / 2, 0), "sea_vs.glslx", "sea_fs.glslx"),
		scene(width, height),
		player(glm::vec3(0, 0, 0)),
		monster(glm::vec3(0,0,-0.1)),
		isMonsterAlive(true),
		width(width),
		height(height)
	{
		int sea_xSize = gameArea.xMax - gameArea.xMin;
		int sea_ySize = gameArea.yMax - gameArea.yMin;

		sea.setSize(sea_xSize, sea_ySize, sea_xSize * 20, sea_ySize * 20);

		IslandFactory factory(9, 9, 0.1, 7, 3);
		islands = factory.getIslands(10);


		player.loadModel("Objects/boat17.obj", "Objects/gun3.obj");


		lightPos = glm::vec3(-5.f, 10.0f, 3.f);

		cameraOffset = glm::vec3(.0f, -1.0f, 3.05f);
		viewPos = cameraOffset;
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
		proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 30.0f);

		originsZ = getOriginsZ(proj * view);

		inverseInitialMvp = glm::inverse(proj * view);

		player.setModel(glm::scale(glm::mat4(), glm::vec3(0.08, 0.08, 0.08)));

		addAllObjectsToScene();
		scene.setLight(lightPos);

		GLController::getInstance().setKeyPressedFunction(keyFunction);
		GLController::getInstance().setMouseTraceFunction(mouseTraceFunction);
		GLController::getInstance().setMousePressFunction(mouseClickFunction);
	}

	~Game()
	{
		for (auto bullet : bullets)
			delete bullet;
	}


	void gameLoop(int frame)
	{
		if (frame == 0 || player.updatePosition())
		{
			updateCamera();
		}

		updatePlayerState();
		player.updateSpeed();

		if (isMonsterAlive)
		{
			//monster.updateMonsterPosition(player.getPosition());
			monster.animate();
		}

		updateVisibleAreas(proj * view);
		scene.updateViewArea();
		scene.renderScene(viewPos, proj * view);

		score.printText2D(std::to_string(glfwGetTime()) + " score", 0.5, 0.5, 30);

		for (std::list<Bullet*>::const_iterator iterator = bullets.begin(), end = bullets.end(); iterator != end;) {
			Bullet* bullet = *iterator;
			if (bullet->shouldRemove() && bullet)
			{
				delete bullet;
				iterator = bullets.erase(iterator);
			}
			else
			{
				bullet->updatePosition();
				bullet->draw(proj * view);
				++iterator;
			}
		}
	}

	bool isGameOver()
	{
		return false;// monster.hasReached(player.getPosition());
	}

	void updateLight()
	{
		lightPos = glm::rotateY(lightPos, (glm::mediump_float)1 / 1000);
		scene.setLight(lightPos);
	}

	bool shouldLightMove()
	{
		return isLightMoving;
	}
};
