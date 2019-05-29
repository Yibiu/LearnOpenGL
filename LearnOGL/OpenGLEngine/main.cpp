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
#include "meshes/sprite_ball.h"


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
	glfwWindowHint(GLFW_SAMPLES, 4);
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
	glEnable(GL_MULTISAMPLE);

	/////////////////////////////////////////////////////
	CGLFactory factory;
	// Shader
	CGLShader *depth_shader_ptr = factory.create_shader("depth");
	depth_shader_ptr->init("./resources/depth.vs", "./resources/depth.fs", true);
	depth_shader_ptr->use();
	depth_shader_ptr->set_int("texture0", 0); // TEXTURE0 -> "texture0"
	CGLShader *single_shader_ptr = factory.create_shader("single");
	single_shader_ptr->init("./resources/depth.vs", "./resources/single.fs", true);
	CGLShader *quad_shader_ptr = factory.create_shader("quad");
	quad_shader_ptr->init("./resources/quad.vs", "./resources/quad.fs", true);
	quad_shader_ptr->use();
	quad_shader_ptr->set_int("texture0", 0); // TEXTURE0 -> "texture0"

	// Texture
	CGLTexture *marble_texture_ptr = factory.create_texture("marble");
	marble_texture_ptr->init("./resources/container.jpg", false, true);
	CGLTexture *metal_texture_ptr = factory.create_texture("metal");
	metal_texture_ptr->init("./resources/container2.png", false, true);

	// Objects
	CGLCube cube;
	cube.init();
	CGLRectangle plane;
	plane.init();
	float quad[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Offscreen buffer
	/*
	GLuint framebuf;
	glGenFramebuffers(1, &framebuf);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
	// create a color attachment texture
	GLuint texture_colorbuf;
	glGenTextures(1, &texture_colorbuf);
	glBindTexture(GL_TEXTURE_2D, texture_colorbuf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colorbuf, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return -1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/
	/////////////////////////////////////////////////////

	// Main loop
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float current_time = glfwGetTime();
	while (!glfwWindowShouldClose(window_ptr))
	{
		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;
		process_input(window_ptr);

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
		//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
		depth_shader_ptr->use();
		depth_shader_ptr->set_mat4("view", camera.get_view());
		depth_shader_ptr->set_mat4("projection", camera.get_perspective());
		// Floor
		glStencilMask(0x00);
		glActiveTexture(GL_TEXTURE0);
		metal_texture_ptr->use();
		depth_shader_ptr->set_mat4("model", glm::mat4(1.0f));
		plane.draw();
		// Cube
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		marble_texture_ptr->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		depth_shader_ptr->set_mat4("model", model);
		cube.draw();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		depth_shader_ptr->set_mat4("model", model);
		cube.draw();
		// Cube Borders
		glStencilMask(0x00);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);
		single_shader_ptr->use();
		single_shader_ptr->set_mat4("view", camera.get_view());
		single_shader_ptr->set_mat4("projection", camera.get_perspective());
		float scale = 1.1f;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		single_shader_ptr->set_mat4("model", model);
		cube.draw();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		single_shader_ptr->set_mat4("model", model);
		cube.draw();
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		//
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);
		//quad_shader_ptr->use();
		//glBindVertexArray(VAO);
		//glBindTexture(GL_TEXTURE_2D, texture_colorbuf);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	/////////////////////////////////////////////////////
	// Shaders
	depth_shader_ptr->uninit();
	factory.destroy_shader(depth_shader_ptr->get_name());

	// Textures
	marble_texture_ptr->uninit();
	factory.destroy_texture(marble_texture_ptr->get_name());
	metal_texture_ptr->uninit();
	factory.destroy_texture(metal_texture_ptr->get_name());

	// Objects
	cube.uninit();
	plane.uninit();
	/////////////////////////////////////////////////////

	// Terminate
	glfwTerminate();

	return 0;
}
