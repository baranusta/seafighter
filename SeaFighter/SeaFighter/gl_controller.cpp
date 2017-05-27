#include "gl_controller.h"
std::function<void(int)> g_keyFunction; 
std::function<void(int, double, double)> g_mouseFunction;

void glErrorChck()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL errorwww: " << err << gluErrorString(err) << std::endl;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		g_keyFunction(key);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		g_mouseFunction(button, xpos, ypos);
	}
}

void GLController::setMousePressFunction(std::function<void(int, double, double)> mouseFunction)
{
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	g_mouseFunction = mouseFunction;
}

void GLController::setKeyPressedFunction(std::function<void(int)> keyFunction)
{
	glfwSetKeyCallback(window, key_callback);
	g_keyFunction = keyFunction;
}

bool GLController::initialize(int width, int height, std::string name)
{
	if (!glfwInit()) {
		return false;
	}
	// Create window and OpenGL 4.3 debug context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (!window) {
		return false;
	}
	// Activate the OpenGL context
	glfwMakeContextCurrent(window);

	// Initialize GLEW extension loader
	glewExperimental = GL_TRUE;
	glewInit();
	return true;
}

void GLController::startGame(std::function<void(void)> gameloop)
{
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gameloop();
		glfwSwapBuffers(window);
	}
}

void GLController::stop()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
