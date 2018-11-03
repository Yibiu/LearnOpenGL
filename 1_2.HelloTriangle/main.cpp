#include <string>
#include <iostream>
#include "glad\glad.h"
#include "GLFW\glfw3.h"
using namespace std;
/**
* @brief:
*	1. Draw triangle using VBO and VAO;
*	2. Draw rect with VBO, VAO and EBO.
*
*	VBO, VAO, EBO, attribute and shander.
*	
* @ref:
*	https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


const char *shander_vs_triangle =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char *shader_fs_triangle =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";



// Window size change callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Window input
void process_input(GLFWwindow *window)
{
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
}


int main()
{
	// Init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window_ptr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (nullptr == window_ptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSetFramebufferSizeCallback(window_ptr, framebuffer_size_callback);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Init OpenGL view port
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// ReSources
	// Shader
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &shander_vs_triangle, NULL);
	glCompileShader(vertex_shader);
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "Vertex Shader Error: " << info_log << endl;
	}
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &shader_fs_triangle, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "Fragment Shader Error: " << info_log << endl;
	}
	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "Link Shader Error: " << info_log << endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	// Triangle
	/*
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	// Left
		0.5f, -0.5f, 0.0f,	// Right
		0.0f,  0.5f, 0.0f	// Top
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO); // Store[...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // ...]Recover
	*/
	// Rect
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	// Right-up
		0.5f, -0.5f, 0.0f,	// Right-down
		-0.5f, -0.5f, 0.0f,	// Left-down
		-0.5f, 0.5f, 0.0f	// Left-up
	};
	unsigned int indices[] = {
		0, 1, 3, // Triangle 1 indexes
		1, 2, 3  // Triangle 2 indexes
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO); // Store[...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // ...]Recover


	// Main loop
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window_ptr))
	{
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Triangle
		/*
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		*/
		// Rect
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Triangle
	/*
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	*/
	// Rect
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate
	glfwTerminate();

	return 0;
}



