#include <stdlib.h>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600


static const char *vs_shader_sources = 
"#version 330\n"
"layout(location = 0) in vec3 Position;\n"
"uniform mat4 gWorld;\n"
"void main()\n"
"{\n"
"	gl_Position = gWorld * vec4(Position, 1.0);\n"
"}\n";

static const char *fs_shader_sources = 
"#version 330\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";


// Window size changed callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
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
	GLFWwindow *window_ptr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOGL", NULL, NULL);
	if (nullptr == window_ptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSetFramebufferSizeCallback(window_ptr, framebuffer_size_callback);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Init OpenGL view port
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/////////////////////////////////////////////////////
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint success;
	GLuint vs_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_shader, 1, &vs_shader_sources, NULL);
	glCompileShader(vs_shader);
	glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar str_err[1024];
		glGetShaderInfoLog(vs_shader, 1024, NULL, str_err);
		std::cout << "Error compiling vertice shader: " << str_err << std::endl;
		return -1;
	}
	GLuint fs_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_shader, 1, &fs_shader_sources, NULL);
	glCompileShader(fs_shader);
	glGetShaderiv(fs_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar str_err[1024];
		glGetShaderInfoLog(fs_shader, 1024, NULL, str_err);
		std::cout << "Error compiling fragment shader: " << str_err << std::endl;
		return -1;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vs_shader);
	glAttachShader(program, fs_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar str_err[1024];
		glGetShaderInfoLog(fs_shader, 1024, NULL, str_err);
		std::cout << "Error compiling shader program: " << str_err << std::endl;
		return -1;
	}
	////////////////////////////////////////////////////

	// Main loop
	while (!glfwWindowShouldClose(window_ptr))
	{
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw...
		glUseProgram(program);
		glBindVertexArray(VAO);

		static float scale = 0.0f;
		scale += 0.001f;
		float translation[4][4];
		translation[0][0] = 1.0f; translation[0][1] = 0.0f; translation[0][2] = 0.0f; translation[0][3] = sinf(scale);
		translation[1][0] = 0.0f; translation[1][1] = 1.0f; translation[1][2] = 0.0f; translation[1][3] = 0.0f;
		translation[2][0] = 0.0f; translation[2][1] = 0.0f; translation[2][2] = 1.0f; translation[2][3] = 0.0f;
		translation[3][0] = 0.0f; translation[3][1] = 0.0f; translation[3][2] = 0.0f; translation[3][3] = 1.0f;
		GLuint world_location = glGetUniformLocation(program, "gWorld");
		glUniformMatrix4fv(world_location, 1, GL_TRUE, &translation[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();

	return 0;
}
