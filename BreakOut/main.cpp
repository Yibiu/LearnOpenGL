#include <string>
#include <iostream>
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "game.h"
using namespace std;
/**
* @brief:
*	Breakout game designed by OpenGL.
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


CNGLGame game;

// Window size change callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			game.set_key(key, KEY_PRESSED);
		}
		else if (action == GLFW_RELEASE) {
			game.set_key(key, KEY_NONE);
		}
	}
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
	glfwSetKeyCallback(window_ptr, key_callback);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Init OpenGL view port
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	game.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Main loop
	GLfloat delta_time = 0.0f;
	GLfloat last_frame = 0.0f;
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window_ptr))
	{
		GLfloat cur_frame = glfwGetTime();
		delta_time = cur_frame - last_frame;
		last_frame = cur_frame;
		game.update(delta_time);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game.render();

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();

	return 0;
}



