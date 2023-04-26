// OGLapp01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MoveLogic.cpp"

// glm files
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VBO, VAO, IBO, shader, uniformModel, uniformProjection;

float triOffset = 0.0f;
bool xDirection = true;


// Vertex Shader
// input of vector with 3 dimensions 
// this will be the final positin in the shader
static const char* vShader = "											\n\
#version 330															\n\
																		\n\
layout (location = 0) in vec3 pos;										\n\
																		\n\
out vec4 vCol;															\n\
																		\n\
uniform mat4 model;														\n\
uniform mat4 projection;												\n\
																		\n\
void main(void){														\n\
	gl_Position = projection * model * vec4(pos, 1.0);								\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);							\n\
}";

//gl_Position = model * vec4(0.4 * pos.x, 0.5 * pos.y, pos.z, 1.0);	\n\

// Fragment shaders
static const char* fShader = "			\n\
#version 330							\n\
										\n\
out vec4 colour;						\n\
										\n\
in vec4 vCol;							\n\
										\n\
void main()								\n\
{										\n\
	colour = vCol;						\n\
}";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// this why we need shader code to pass the size of it
	// also the string include at the top come in handy here
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// This is for debugging purposes
	GLint result = 0;
	GLchar eLog[1024] = {  };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d the shader: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("error creating shader");
		return;
	}

	// type is to identify the openGL which shader type is this
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	// This is for debugging purposes
	GLint result = 0;
	GLchar eLog[1024] = {  };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking the program: %s \n", eLog);
		return;
	}


	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating the program: %s \n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}

void CreateTriangle()
{
	// 0 3 1 means that first draw the first vector in the vertices then last and then second
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f,-1.0f, 0.0f,
		0.0f,-1.0f, 1.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};

	// Generating a VAO array
	glGenVertexArrays(1, &VAO);
	// Binding it with an ID
	glBindVertexArray(VAO);

	// Generating and binding IBO buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1.Generating VBO buffers 2.Binding it with an ID
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Filling it with actual data
	// Static draw means we do not want to change it later
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// first is location, second is number of values which is 3: xyz, fifth is stride which is jumping values  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbinding values
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

int InitilizeGlfw()
{
	// Initilize GLFW
	if (!glfwInit())
	{
		cout << "Failed initilization!!";
		glfwTerminate();
		return -1;
	}

	// Setting up GLFW
	// telling the opneGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// detects old openGL codes, NO backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

}

int main()
{
	if (InitilizeGlfw() == -1) { return 1; };

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "The Window", nullptr, nullptr);

	if (!mainWindow)
	{
		printf("failed to open the window");
		glfwTerminate();
		return 1;
	}

	// Get buffer size infos
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern features of glew
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
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

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	Movement xMovement;

	// Loop until the window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user inputs
		glfwPollEvents();

		xMovement.move(xDirection, triOffset);

		// Clear window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// it will use the shader we compiled up there which is global
		glUseProgram(shader);

		// Should be initilized first 
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		// Rotate but disturb the triangle
		model = glm::rotate(model, triOffset * 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Scale the triangle
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// using model matrix to translate the tr.iangle || false is for disabling the transform
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		// DEPRECATED:
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		// NEW: 
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Unbinding the program
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;

}

// Tips for Getting Started: 
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
