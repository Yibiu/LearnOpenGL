#include <stdlib.h>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "control/camera.h"
#include "meshes/factory.h"
#include "meshes/sprite_triangle.h"
#include "meshes/sprite_rect.h"
#include "meshes/sprite_cube.h"


#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600


float last_time = 0.0f;
float delta_time = 0.0f;
CGLCamera camera(WINDOW_WIDTH, WINDOW_HEIGHT);


// Window size changed callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Window key input
void process_input(GLFWwindow *window)
{
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.update_moving(CGLCamera::MOVING_FRONT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.update_moving(CGLCamera::MOVING_BACK, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.update_moving(CGLCamera::MOVING_LEFT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.update_moving(CGLCamera::MOVING_RIGHT, delta_time);
	}
}

// Mouse rotating
void mouse_rotating_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.update_rotating(xpos, ypos);
}

// Mouse scrolling
void mouse_scrolling_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.update_scrolling(xoffset, yoffset);
}


int main()
{
	// Init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window_ptr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLEngine", NULL, NULL);
	if (nullptr == window_ptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSetFramebufferSizeCallback(window_ptr, framebuffer_size_callback);
	glfwSetCursorPosCallback(window_ptr, mouse_rotating_callback);
	glfwSetScrollCallback(window_ptr, mouse_scrolling_callback);
	//glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	CGLShader *world_shader_ptr = factory.create_shader("world");
	world_shader_ptr->init("./resources/world.vs", "./resources/tex.fs", true);
	world_shader_ptr->use();
	world_shader_ptr->set_int("texture0", 0); // TEXTURE0 -> "texture0"

	CGLTexture *face_texture_ptr = factory.create_texture("face");
	face_texture_ptr->init("./resources/face.png", false, true);

	CGLTriangle triangle;
	triangle.init();
	CGLRectangle rectangle;
	rectangle.init();
	CGLCube cube0;
	cube0.init();
	glm::mat4 model0(1.0f);
	model0 = glm::translate(model0, glm::vec3(1.0f, 0.0f, 0.0f));
	cube0.set_model(model0);
	CGLCube cube1;
	cube1.init();
	glm::mat4 model1(1.0f);
	model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));
	cube1.set_model(model1);
	/////////////////////////////////////////////////////

	// Main loop
	glEnable(GL_DEPTH_TEST);
	float current_time = glfwGetTime();
	while (!glfwWindowShouldClose(window_ptr))
	{
		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw...
		//raw_shader_ptr->use();
		//triangle.draw();
		//
		//tex_shader_ptr->use();
		//glActiveTexture(GL_TEXTURE0);
		//face_texture_ptr->use();
		//rectangle.draw();
		//
		world_shader_ptr->use();
		world_shader_ptr->set_mat4("model", cube0.get_model());
		world_shader_ptr->set_mat4("view", camera.get_view());
		world_shader_ptr->set_mat4("projection", camera.get_perspective());
		glActiveTexture(GL_TEXTURE0);
		face_texture_ptr->use();
		cube0.draw();
		world_shader_ptr->set_mat4("model", cube1.get_model());
		cube1.draw();

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	/////////////////////////////////////////////////////
	raw_shader_ptr->uninit();
	factory.destroy_shader(raw_shader_ptr->get_name());
	tex_shader_ptr->uninit();
	factory.destroy_shader(tex_shader_ptr->get_name());
	world_shader_ptr->uninit();
	factory.destroy_shader(world_shader_ptr->get_name());

	face_texture_ptr->uninit();
	factory.destroy_texture(face_texture_ptr->get_name());

	triangle.uninit();
	rectangle.uninit();
	cube0.uninit();
	cube1.uninit();
	/////////////////////////////////////////////////////

	// Terminate
	glfwTerminate();

	return 0;
}
