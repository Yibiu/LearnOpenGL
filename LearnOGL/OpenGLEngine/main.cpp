#include <stdlib.h>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "control/camera.h"
#include "lights/light_dot.h"
#include "meshes/factory.h"
#include "meshes/sprite_triangle.h"
#include "meshes/sprite_rect.h"
#include "meshes/sprite_cube.h"
#include "meshes/sprite_conus.h"
#include "meshes/sprite_line.h"
#include "meshes/sprite_circle.h"
#include "meshes/sprite_cylinder.h"
#include "meshes/sprite_plane.h"


#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600


float last_time = 0.0f;
float delta_time = 0.0f;
CGLCamera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
CGLDotLight light;


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
	// Light
	light.set_position(glm::vec3(8.0f, 8.0f, 0.0f));
	light.set_ambient(glm::vec3(0.2f, 0.2f, 0.2f));
	light.set_diffuse(glm::vec3(0.8f, 0.8f, 0.8f));
	light.set_specular(glm::vec3(1.0f, 1.0f, 1.0f));
	light.set_constant(1.0f);
	light.set_linear(0.045f);
	light.set_quadratic(0.0075f);

	CGLFactory factory;
	// Shader
	CGLShader *conus_shader_ptr = factory.create_shader("conus");
	conus_shader_ptr->init("./resources/conus.vs", "./resources/conus.fs", true);
	// Shader
	CGLShader *lamp_shader_ptr = factory.create_shader("lamp");
	lamp_shader_ptr->init("./resources/lamp.vs", "./resources/lamp.fs", true);
	CGLShader *obj_shader_ptr = factory.create_shader("object");
	obj_shader_ptr->init("./resources/material.vs", "./resources/material.fs", true);
	obj_shader_ptr->use();
	obj_shader_ptr->set_int("texture0", 0); // TEXTURE0 -> "texture0"

	// Texture(container2.png)
	CGLTexture *container_texture_ptr = factory.create_texture("container");
	container_texture_ptr->init("./resources/container2.png", false, true);

	// Objects
	CGLCube lamp;
	lamp.init();
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::translate(model, light.get_position());
	lamp.set_model(model);
	CGLCube object;
	object.init();
	object.set_ambient(glm::vec3(1.0f, 1.0f, 1.0f));
	object.set_diffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	object.set_specular(glm::vec3(1.0f, 1.0f, 1.0f));
	object.set_shininess(32.0f);
	CGLPlane plane;
	plane.init();
	/////////////////////////////////////////////////////

	// Main loop
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		/*
		// Lamp
		lamp_shader_ptr->use();
		lamp_shader_ptr->set_mat4("model", lamp.get_model());
		lamp_shader_ptr->set_mat4("view", camera.get_view());
		lamp_shader_ptr->set_mat4("projection", camera.get_perspective());
		lamp.draw();
		// Object
		obj_shader_ptr->use();
		obj_shader_ptr->set_mat4("model", object.get_model());
		obj_shader_ptr->set_mat4("view", camera.get_view());
		obj_shader_ptr->set_mat4("projection", camera.get_perspective());
		obj_shader_ptr->set_vec3f("camera_pos", camera.get_position());
		obj_shader_ptr->set_vec3f("lamp.position", light.get_position());
		obj_shader_ptr->set_vec3f("lamp.ambient", light.get_ambient());
		obj_shader_ptr->set_vec3f("lamp.diffuse", light.get_diffuse());
		obj_shader_ptr->set_vec3f("lamp.specular", light.get_specular());
		obj_shader_ptr->set_float("lamp.constant", light.get_constant());
		obj_shader_ptr->set_float("lamp.linear", light.get_linear());
		obj_shader_ptr->set_float("lamp.quadratic", light.get_quadratic());
		obj_shader_ptr->set_vec3f("obj.ambient", object.get_ambient());
		obj_shader_ptr->set_vec3f("obj.diffuse", object.get_diffuse());
		obj_shader_ptr->set_vec3f("obj.specular", object.get_specular());
		obj_shader_ptr->set_float("obj.shininess", object.get_shininess());
		glActiveTexture(GL_TEXTURE0);
		container_texture_ptr->use();
		object.draw();
		*/
		conus_shader_ptr->use();
		conus_shader_ptr->set_mat4("model", plane.get_model());
		conus_shader_ptr->set_mat4("view", camera.get_view());
		conus_shader_ptr->set_mat4("projection", camera.get_perspective());
		plane.draw();

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	/////////////////////////////////////////////////////
	// Shaders
	lamp_shader_ptr->uninit();
	factory.destroy_shader(lamp_shader_ptr->get_name());
	obj_shader_ptr->uninit();
	factory.destroy_shader(obj_shader_ptr->get_name());

	// Textures
	container_texture_ptr->uninit();
	factory.destroy_texture(container_texture_ptr->get_name());

	// Objects
	lamp.uninit();
	object.uninit();
	plane.uninit();
	/////////////////////////////////////////////////////

	// Terminate
	glfwTerminate();

	return 0;
}
