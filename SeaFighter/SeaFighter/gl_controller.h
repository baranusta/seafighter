#pragma once

#include <gl\glew.h>
#include <gl\freeglut.h>
#include <GLFW\glfw3.h>

#include <functional>
#include <iostream>

class GLController
{
	GLFWwindow* window;
	

	GLController() {}

public:
	GLController(GLController const&) = delete;
	void operator=(GLController const&) = delete;

	static GLController& getInstance()
	{
		static GLController instance;
		return instance;
	}

	void setMousePressFunction(std::function<void(int, double, double)> mouseFunction);
	void setKeyPressedFunction(std::function<void(int)> keyFunction);
	bool initialize(int width, int height, std::string name);
	void startGame(std::function<void(void)> gameloop);
	void stop();
};