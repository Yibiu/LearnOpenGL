#include <string>
#include <iostream>
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "shader.h"
using namespace std;
/**
* @brief:
*	The triangle with the basic shader methods.
*	1. uniform;
*	2. more attribute;
*	3. shader class.
*
* @ref:
*	https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


// Uniform
/*
const char *shander_vs_triangle =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"}\n";

const char *shader_fs_triangle =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\n";
*/
// More attribute
const char *shander_vs_triangle = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char *shader_fs_triangle = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";


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
	/*
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
	*/
	CShader shader;
	shader.compile("./shader.vs", "./shader.fs");
	// Uniform
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
	// More attribute
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// Right-down
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// Left-down
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f		// Top
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO); // Store[...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // ...]Recover


	// Main loop
	while (!glfwWindowShouldClose(window_ptr))
	{
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniform
		/*
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		float cur_time = glfwGetTime();
		float green = sin(cur_time) / 2.0f + 0.5f;
		int color_location = glGetUniformLocation(shader_program, "ourColor");
		glUniform4f(color_location, 0.0f, green, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		*/
		// More attribute
		//glUseProgram(shader_program);
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Uniform
	/*
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	*/
	// More attribute
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate
	glfwTerminate();

	return 0;
}



