#include "gl_controller.h"
#include "game.h"

#include <iostream>
#include <iomanip>
#include <vector>


int main()
{
	int width = 600;
	int height = 500;

	if (GLController::getInstance().initialize(width, height, "Sea Fighter"))
	{
		GLenum err;

		try {
			Game seaFighter(width, height);

			float frame = 0;
			float time = 0;
			GLController::getInstance().startGame([&]() {
				if (!seaFighter.isGameOver())
				{
					if (frame == 0 || seaFighter.shouldLightMove())
					{
						seaFighter.updateLight();
					}
					seaFighter.gameLoop(frame);
				}
				else
				{
					GLController::getInstance().endTheGame();
					std::cout << "                                                         \n";
					std::cout << "                                                         \n";
					std::cout << "                       GAME OVER                         \n";
					std::cout << "                                                         \n";
					std::cout << "                                                         \n";
				}
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