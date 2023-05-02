#pragma once

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffer() { glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;

	GLint width{ 0 }, height{ 0 };
	GLint bufferWidth{ 0 }, bufferHeight{ 0 };

	// Covering range of ascii codes
	// to check the key pressed
	bool keys[1024];

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};