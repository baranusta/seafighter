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

	Text score;
	Scene scene;
	Sea sea;
	Monster monster;
	bool isMonsterAlive;

	std::vector<Island> islands;
	std::list<Bullet*> bullets;
	std::vector<std::vector<double>> valuesHeightMap;
	

	glm::vec3 lightPos;
	glm::vec3 viewPos;
	glm::vec3 cameraOffset;

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 inverseInitialMvp;

	bool isLightMoving = false;
	int width, height;
	float oldPosX;
	float oldPosY;

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

	bool obtainPlayerMatrix(glm::vec3 playerPosition) {
		int cubeX = (int)playerPosition.x / 0.1;
		int cubeY = (int)playerPosition.y / 0.1;

		if (valuesHeightMap[cubeX + 1][cubeY + 1] != 0 |
			valuesHeightMap[cubeX - 1][cubeY - 1] != 0 |
			valuesHeightMap[cubeX + 1][cubeY - 1] != 0 |
			valuesHeightMap[cubeX - 1][cubeY + 1] != 0) {

			return true;
		}
		else {
			return false;
		}

	}


public:

	Game(int width, int height) :
		score("Holstein.DDS"),
		sea(glm::vec3(0, 1.5, 0), "sea_vs.glslx", "sea_fs.glslx"),
		scene(width, height),
		player(glm::vec3(0, 0, 0)),
		monster(glm::vec3(0, 3, -0.1)),
		isMonsterAlive(true),
		width(width),
		height(height)
	
	{
		sea.setSize(20, 20, 500, 500);
		oldPosX = oldPosY = 0.0;
		IslandFactory factory(9, 9, 0.1, 7, 3);
		islands = factory.getIslands(10);
		bool boleanMap[9][9];
		memset(boleanMap, 0, sizeof(boleanMap));
		//valuesHeightMap = factory.getHeightMap();
		//valuesHeightMap = factory.getHeightMap();
		

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
		return monster.hasReached(player.getPosition()) | obtainPlayerMatrix(player.getPosition());
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

#define unitrid 0.1
	
	void isBoatCollideIsland(glm::vec3 playerPosition, std::vector<std::vector<double>> map, std::vector<Island> islands)
	{
		bool visited[5][5];
		memset(visited, 0, sizeof(visited));

		int count = 0;
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (map[i][j] && !visited[i][j]) // If a cell with value 1 is not
				{							// visited yet, then new island found
					//DFS(map, i, j, visited);     // Visit all cells in this island.
					++count;                   // and increment island count
				}
			}
		}

	}



#define ROW 5 //Must be the size of the all hight map of boleans
#define COL 5

	// A function to check if a given cell (row, col) can be included in DFS
	int isSafe(int M[][COL], int row, int col, bool visited[][COL])
	{
		// row number is in range, column number is in range and value is 1 
		// and not yet visited
		return (row >= 0) && (row < ROW) &&
			(col >= 0) && (col < COL) &&
			(M[row][col] && !visited[row][col]);
	}

	// A utility function to do DFS for a 2D boolean matrix. It only considers
	// the 8 neighbours as adjacent vertices
	void DFS(int M[][COL], int row, int col, bool visited[][COL])
	{
		// These arrays are used to get row and column numbers of 8 neighbours 
		// of a given cell
		static int rowNbr[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
		static int colNbr[] = { -1,  0,  1, -1, 1, -1, 0, 1 };

		// Mark this cell as visited
		visited[row][col] = true;

		// Recur for all connected neighbours
		for (int k = 0; k < 8; ++k)
			if (isSafe(M, row + rowNbr[k], col + colNbr[k], visited))
				DFS(M, row + rowNbr[k], col + colNbr[k], visited);
	}



};


