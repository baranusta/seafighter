#pragma once

#include "gl_controller.h"

#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "player.h"
#include "monster.h"
#include "scene.h"
#include "bullet.h"

#include <list>



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

extern glm::vec4 getWorldCoordinate(glm::mat4 matrix, int xPos, int yPos, int width, int height);

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


	void addAllObjectsToScene()
	{
		scene.addPlayer(&player, proj * view);
		scene.addChildForRender(&sea);
		scene.addChildForRender(&monster);
		for (auto & island : islands)
		{
			scene.addChildForRender(&island);
			scene.addChildForShadow(&island);
		}

	}

	std::function<void(int, double, double)> mouseClickFunction = [&](int button, double xpos, double ypos) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			glm::vec3 direction = player.getGunDirection();
			bullets.push_back(new Bullet(player.getPosition(), direction, islands));
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
			player.rotate(-0.03);
		if (p_keyboardStates.rotateRight)
			player.rotate(0.03);
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
		monster(glm::vec3(0,3,0)),
		isMonsterAlive(false),
		width(width),
		height(height)
	{
		sea.setSize(20, 20, 500, 500);

		IslandFactory factory(9, 9, 0.1, 7, 3);
		islands = factory.getIslands(10);


		player.loadModel("Objects/boat17.obj", "Objects/gun3.obj");


		lightPos = glm::vec3(-5.f, 10.0f, 3.f);

		cameraOffset = glm::vec3(.0f, -2.0f, 2.05f);
		viewPos = cameraOffset;
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
		proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 30.0f);

		inverseInitialMvp = glm::inverse(proj * view);

		player.setModel(glm::scale(glm::mat4(), glm::vec3(0.08, 0.08, 0.08)));

		addAllObjectsToScene();

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
			monster.updateMonsterPosition(player.getPosition());
			monster.animate();
		}
		
		scene.renderScene(viewPos, proj * view);

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
		return monster.hasReached(player.getPosition());
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
