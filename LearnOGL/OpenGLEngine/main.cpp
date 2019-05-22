#include <stdlib.h>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "meshes/factory.h"
#include "meshes/sprite_triangle.h"
#include "meshes/sprite_rect.h"


#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600


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
	CGLFactory factory;
	CGLShader *raw_shader_ptr = factory.create_shader("raw");
	raw_shader_ptr->init("./resources/raw.vs", "./resources/raw.fs", true);
	CGLShader *tex_shader_ptr = factory.create_shader("tex");
	tex_shader_ptr->init("./resources/tex.vs", "./resources/tex.fs", true);
	tex_shader_ptr->use();
	tex_shader_ptr->set_int("texture0", 0); // TEXTURE0 -> "texture0"

	CGLTexture *face_texture_ptr = factory.create_texture("face");
	face_texture_ptr->init("./resources/face.png", false, true);

	CGLTriangle triangle;
	triangle.init();
	CGLRectangle rectangle;
	rectangle.init();
	/////////////////////////////////////////////////////

	// Main loop
	while (!glfwWindowShouldClose(window_ptr))
	{
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw...
		//
		//raw_shader_ptr->use();
		//triangle.draw(NULL);
		//
		tex_shader_ptr->use();
		glActiveTexture(GL_TEXTURE0);
		face_texture_ptr->use();
		rectangle.draw(NULL);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	raw_shader_ptr->uninit();
	factory.destroy_shader("raw");
	tex_shader_ptr->uninit();
	factory.destroy_shader("tex");

	face_texture_ptr->uninit();
	factory.destroy_texture("face");

	triangle.uninit();
	rectangle.uninit();

	// Terminate
	glfwTerminate();

	return 0;
}
