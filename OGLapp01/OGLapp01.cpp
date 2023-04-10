// OGLapp01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VBO, VAO, shader;

// Vertex Shader
// input of vector with 3 dimensions 
// this will be the final positin in the shader
static const char* vShader = "			\n\
#version 330							\n\
										\n\
layout (location = 0) in vec3 pos;		\n\
void main(){							\n\
	gl_Position = vec4(0.4 * pos.x, 0.5 * pos.y, pos.z, 1.0);\n\
}";

// Fragment shaders
static const char* fShader = "			\n\
#version 330							\n\
										\n\
out vec4 colour;						\n\
										\n\
void main()								\n\
{										\n\
	colour = vec4(0.0, 1.0, 0.0, 1.0);	\n\
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
}

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f,-1.0f, 0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};

	// Generating a VAO array
	glGenVertexArrays(1, &VAO);
	// Binding it with an ID
	glBindVertexArray(VAO);

	// Generating VBO buffers
	glGenBuffers(1, &VBO);
	// Binding it with an ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Filling it with actual data
	// Static draw means we do not want to change it later
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// first is location, second is number of values which is 3: xyz, fifth is stride which is jumping values  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbinding values
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

int main()
{
	// Initilize GLFW
	if (!glfwInit())
	{
		cout << "Failed initilization!!";
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

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Loop until the window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user inputs
		glfwPollEvents();

		// Clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// it will use the shader we compiled up there which is global
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Unbinding the program
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;

}

// Tips for Getting Started: 
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
