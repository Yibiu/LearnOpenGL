#include <string>
#include <iostream>
#include <vector>
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "shader.h"
using namespace std;
/**
* @brief:
*	Cube maps.
*
*	1. Sky box.
*	2. CubeMap is sampled by direction vector.(No need for texture corrdinates).
*
* @ref:
*	https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/06%20Cubemaps/
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


float delta_time = 0.0f, last_frame = 0.0f;
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool first_mouse = true;
float last_mouse_x = 800.0f / 2.0;
float last_mouse_y = 600.0f / 2.0;


// Window size change callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Window input
void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float speed = 2.5 * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed;
}

// Mouse move
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse) {
		last_mouse_x = xpos;
		last_mouse_y = ypos;
		first_mouse = false;
	}

	float offset_x = xpos - last_mouse_x;
	float offset_y = ypos - last_mouse_y;
	last_mouse_x = xpos;
	last_mouse_y = ypos;

	float sensitivity = 0.1f;
	offset_x *= sensitivity;
	offset_y *= sensitivity;
	yaw += offset_x;
	pitch -= offset_y;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front(1.0f);
	front.x = cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
}

// Mouse scroll
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
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
	glfwSetCursorPosCallback(window_ptr, mouse_move_callback);
	glfwSetScrollCallback(window_ptr, mouse_scroll_callback);
	//glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Init OpenGL view port
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// ReSources
	// Shader
	CShader shader;
	shader.compile("./shader.vs", "./shader.fs");
	// Skybox
	float skybox_vertices[] = {
		// Face1
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		// Face2
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		// Face3
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		// Face4
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		// Face5
		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		// Face6
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO); // Store[...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); // ...]Recover
	// Texture
	vector<string> cube_images = {
		"./skybox/right.jpg",		// Right
		"./skybox/left.jpg",		// Left
		"./skybox/top.jpg",			// Up
		"./skybox/bottom.jpg",		// Down
		"./skybox/front.jpg",		// Back
		"./skybox/back.jpg"			// Front
	};
	unsigned int tex_cube;
	glGenTextures(1, &tex_cube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	for (uint32_t i = 0; i < cube_images.size(); i++) {
		int width, height, nrChannels;
		unsigned char *data = stbi_load(cube_images[i].c_str(), &width, &height, &nrChannels, 0);
		if (NULL != data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	shader.use();
	shader.set_int("skybox", 0);

	// Main loop
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window_ptr))
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 model(1.0), view(1.0), projection(1.0);
		model = glm::scale(model, glm::vec3(20.0));
		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		unsigned int model_location = glGetUniformLocation(shader.get_id(), "model");
		unsigned int view_location = glGetUniformLocation(shader.get_id(), "view");
		unsigned int proj_location = glGetUniformLocation(shader.get_id(), "projection");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate
	glfwTerminate();

	return 0;
}



