#pragma once

#include "gl_controller.h"

#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "mine_factory.h"
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
	enum IntersectFace{ WEST, NORTH, EAST, SOUTH, NONE };
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
	std::vector<Mine*> mines;
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
		scene.addPlayer(&player);
		scene.addChildForRender(&sea);
		scene.addChildForRender(&monster);
		for (auto & island : islands)
		{
			scene.addChildForRender(&island);
			scene.addChildForShadow(&island);
		}

		for (auto & mine : mines)
		{
			scene.addChildForRender(mine);
		}

	}

	std::function<void(int, double, double)> mouseClickFunction = [&](int button, double xpos, double ypos) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			glm::vec3 direction = player.getGunDirection();
			Bullet* bullet = new Bullet(player.getPosition(), direction, islands);
			bullet->isHittingAnyMine(mines);
			bullets.push_back(bullet);
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
			player.rotate(-0.1);
		if (p_keyboardStates.rotateRight)
			player.rotate(0.1);
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

	IntersectFace isOutBoundaries(glm::vec3 pos)
	{
		if (pos[0] > gameArea.xMax - 1)
			return EAST;
		if (pos[0] < gameArea.xMin + 1)
			return WEST;
		if (pos[1] > gameArea.yMax - 1)
			return NORTH;
		if (pos[1] < gameArea.yMin + 1)
			return SOUTH;
		return NONE;
	}

	void repositionPlayer(IntersectFace intersectedFace, glm::vec3 vec)
	{
		switch (intersectedFace)
		{

		case EAST:
			player.updatePosition(glm::vec3(-vec[0] + gameArea.xMin, 0, 0));
			break;
		case WEST:
			player.updatePosition(glm::vec3(-vec[0] + gameArea.xMax, 0, 0));
			break;
		case NORTH:
			player.updatePosition(glm::vec3(0, -vec[1] + gameArea.yMin, 0));
			break;
		case SOUTH:
			player.updatePosition(glm::vec3(0, -vec[1] + gameArea.yMax, 0));
			break;
		}

	}

	void getIntoBoundaries()
	{
		IntersectFace intersectLeft = isOutBoundaries(visibleTopLeft);
		IntersectFace intersectRight = isOutBoundaries(visibleTopRight);
		if (intersectLeft != NONE)
		{
			repositionPlayer(intersectLeft, visibleBottomRight);
		}
		else if(isOutBoundaries(visibleTopRight) != NONE)
		{
			repositionPlayer(intersectRight, visibleBottomLeft);
		}
	}

public:

	Game(int width, int height) :
		gameArea(BBox(-15.,+15.,-15.,+15.)),
		score("Holstein.DDS"),
		sea(glm::vec3((gameArea.xMax - gameArea.xMin)/2, (gameArea.yMax - gameArea.yMin) / 2, 0), "sea_vs.glslx", "sea_fs.glslx"),
		scene(width, height),
		player(glm::vec3(0, 0, 0)),
		monster(glm::vec3(0,3,-0.1)),
		isMonsterAlive(false),
		width(width),
		height(height)
	{
		int sea_xSize = gameArea.xMax - gameArea.xMin;
		int sea_ySize = gameArea.yMax - gameArea.yMin;

		sea.setSize(sea_xSize, sea_ySize, sea_xSize * 20, sea_ySize * 20);

		IslandFactory i_factory(11, 11, 0.1, 7, 3);
		islands = i_factory.getIslands(10);

		MineFactory m_factory(BBox(-6,6,-6,6));
		mines = m_factory.generateMines(10, islands);

		player.loadModel("Objects/boat-b.obj", "Objects/gun-b.obj");
		monster.setVisibility(false);


		lightPos = glm::vec3(-5.f, 5.0f, 3.f);

		cameraOffset = glm::vec3(.0f, -1.0f, 3.05f);
		viewPos = cameraOffset;
		view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
		proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 30.0f);

		originsZ = getOriginsZ(proj * view);

		inverseInitialMvp = glm::inverse(proj * view);


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
		for (auto mine : mines)
			delete mine;
	}


	void gameLoop(int frame)
	{
		updateCamera();
		updatePlayerState();
		player.updateSpeed();
		player.updatePosition();
		

		if (isMonsterAlive)
		{
			monster.updateMonsterPosition(player.getPosition());
			monster.animate();
		}

		updateVisibleAreas(glm::inverse(proj * view));
		getIntoBoundaries();

		scene.updateViewArea(player.getPosition());
		scene.renderScene(viewPos, proj * view);
		printError("saaaaaaa");

		int aliveMineCount = 0;
		int i = 0;
		for (auto& mine : mines)
		{
			if (mine->isExist() && i != mines.size() - 1)
				aliveMineCount++;
			i++;
		}

		if (!aliveMineCount || glfwGetTime()>80)
		{
			player.reset();
			isMonsterAlive = true;

			monster.setVisibility(true);
			for (auto& island : islands)
			{
				island.setVisibility(false);
			}
			for (auto& mine : mines)
			{
				mine->setVisibility(false);
			}
		}
		else
		{
			for (auto& mine : mines)
			{
				mine->update();
			}
		}
		#ifndef DEBUG_SHADOW
				score.printText2D("Remaining Mine count is " + std::to_string(aliveMineCount), 0.5, 0.5, 30);
		#endif

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

	bool isPlayerCollided()
	{
		auto& points= player.getPoints();
		for (auto& island : islands)
		{
			for (auto bbox : island.getBBox())
			{
				if (bbox.isColliding(points))
					return true;
			}
		}
		int i = 0;
		for (auto& mine : mines)
		{
			if (mine->isExist() && i!= mines.size())
			{
				if (mine->getBBox().isColliding(points))
					return true;
			}
			i++;
		}
		return false;
	}

	bool isGameOver()
	{
		return isPlayerCollided() || isMonsterAlive && monster.hasReached(player.getPosition());
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
