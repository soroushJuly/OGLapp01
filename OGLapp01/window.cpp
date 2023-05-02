#include <iostream>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm files
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"

Window::Window()
{
	width = 600;
	height = 800;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	// Initilize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed initilization!!";
		glfwTerminate();
		return 1;
	}

	// Setting up GLFW
	// telling the opneGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// detects old openGL codes, NO backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window
	GLFWwindow* mainWindow = glfwCreateWindow(width, height, "The Window", nullptr, nullptr);
	if (!mainWindow)
	{
		printf("failed to open the window");
		glfwTerminate();
		return 1;
	}

	// Get buffer size infos
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// handle key and mouse input
	createCallbacks();

	// Allow modern features of glew
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("glew failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// to solve the problem of not showing blue color in front
	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// We assign a user pointer to this window
	glfwSetWindowUserPointer(mainWindow, this);


}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// After this the glfwWindowShouldClose will be called
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// A key that we can handle
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("released: %d\n", key);
		}
	}
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
