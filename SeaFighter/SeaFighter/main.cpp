#include "gl_controller.h"
#include "game.h"

#include <iostream>
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
			GLController::getInstance().startGame([&]() {
				if (frame == 0 || seaFighter.shouldLightMove())
				{
					seaFighter.updateLight();
				}
				seaFighter.gameLoop(frame);
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