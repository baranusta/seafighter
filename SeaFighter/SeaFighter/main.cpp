#include "gl_controller.h"
#include "game_object.h"
#include "sea.h"
#include "island_factory.h"
#include "player.h"
#include "naval_mine.h"
#include "naval_mine_factory.h"
#include <typeinfo>


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

struct PlayerKeyboardStates {
	bool rotateLeft;
	bool rotateRight;
	bool isAccelerating;
	bool isDecelerating;
	
	PlayerKeyboardStates() : rotateLeft(false), rotateRight(false),
							isAccelerating(false), isDecelerating(false)
	{}
};

void updatePlayerState(Player & player, PlayerKeyboardStates states)
{
	if (states.isAccelerating)
		player.accelerate();
	if (states.isDecelerating)
		player.decelerate();
	if (states.rotateLeft)
		player.rotate(-0.01);
	if (states.rotateRight)
		player.rotate(0.01);
}

glm::vec4 getWorldCoordinate(glm::mat4 matrix, int xPos, int yPos, int width, int height)
{
	glm::vec4 screenPos(2 * xPos / (float)width - 1, 1 - 2 * yPos / (float)height, 0.94975, 1.0);
	glm::vec4 worldPos = matrix * screenPos;
	worldPos /= worldPos[3];
	std::cout << std::setw(8) << std::setprecision(3) << screenPos[0] << " " << screenPos[1] << " worldPos: "<< worldPos[0] << " " << worldPos[1] << std::endl;
	return worldPos;
}

int main()
{
	int width = 600;
	int height = 500;

	if (GLController::getInstance().initialize(width, height, "Sea Fighter"))
	{
		GLenum err;
		setMouseFunctions();
		//GameObject boat(glm::vec3(0,0,0));
		//boat.loadModel("Objects/DavidHeadCleanMax.obj");
		try {

			Sea see(glm::vec3(0, 1.5, 0), "sea_vs.glslx", "sea_fs.glslx");
			see.setSize(20, 20, 500, 500);

			IslandFactory factory(9, 9, 0.1, 7, 3);
			auto islands = factory.getIslands(10);
			

			//Creat One Enemy
			Mine newEnemy(glm::vec3(0,0,0));
			newEnemy.loadModel();
			float size = 0.0f;

			//Create Enemies //There is now height here.
			//Set almost same parameters as the objects IslandFactory
			MineFactory enemyFactory(9, 9, 0.1, 7, 3);
			auto enemies = enemyFactory.buildEnemies(islands,10);
			for (auto & enemy : enemies)
				enemy.loadModel();

			Player player(glm::vec3(0, 0, 0));
			player.loadModel("Objects/boat_new.obj", "Objects/gun_new.obj");
			PlayerKeyboardStates p_keyboardStates;


			glm::vec3 light = glm::vec3(-5.0f, 10.0f, 5.0f);
			bool isLightMoving = false;

			glm::vec3 offset = glm::vec3(.0f, -2.0f, 2.05f);
			glm::vec3 viewPos = offset;
			glm::mat4 view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 proj = glm::perspective(45.0f, width / static_cast<float>(height), 0.1f, 10.0f);
			glm::mat4 model = glm::mat4();
			glm::mat4 initialMvp = proj * view * model;

			glm::mat4 inverseInitialMVP = glm::inverse(initialMvp);


			player.setMvp(glm::scale(proj * view, glm::vec3(0.05, 0.05, 0.05)));
			
			
			GLController::getInstance().setMouseTraceFunction([&](double xPos, double yPos) {
				glm::vec4 worldPos = getWorldCoordinate(inverseInitialMVP,xPos,yPos,width,height);
				player.rotateGunTo(worldPos);
			});

			GLController::getInstance().setKeyPressedFunction([&](int button, bool isPressed) {
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
				case GLFW_KEY_R:
					size += 0.15;
					newEnemy.updateTexture("toon_map");
					break;
				case GLFW_KEY_E:
					size -= 0.15f;
				}
			});

			float frame = 0;
			GLController::getInstance().startGame([&]() {

				if (isLightMoving)
					light = glm::rotateY(light, (glm::mediump_float)1 / 10000);


				//update mvp
				viewPos = glm::rotate(offset, (glm::mediump_float)player.getRotation(), glm::vec3(0, 0, -1)) + player.getPosition();
				view = glm::lookAt(viewPos, player.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 mvp = proj * view * model;

				//Physics functions
				updatePlayerState(player, p_keyboardStates);
				player.updateSpeed();
				player.updatePosition();


				//draw functions
				player.draw(viewPos, light);
				see.draw(mvp, viewPos, light);
				newEnemy.draw(mvp,viewPos,glfwGetTime(), glm::vec3(0,0,0),size);
				for (auto & island : islands)
					island.draw(mvp, viewPos, light);
				for (auto & enemy : enemies)
					enemy.draw(mvp, viewPos, glfwGetTime(), glm::vec3(1, 1, 0), 0.009);
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