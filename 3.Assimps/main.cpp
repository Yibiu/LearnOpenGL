#include <string>
#include <iostream>
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "shader.h"
#include "model.h"
using namespace std;
/**
* @brief:
* Mesh.
* Load models.
*
* @ref:
*	https://learnopengl-cn.github.io/03%20Model%20Loading/01%20Assimp/
*	https://learnopengl-cn.github.io/03%20Model%20Loading/02%20Mesh/
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


float delta_time = 0.0f, last_frame = 0.0f;
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
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
	shader.compile("./model.vs", "./model.fs");
	// Model
	CNGLModel models;
	models.load("./nanosuit/nanosuit.obj");

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

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		shader.use();
		glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "view"), 1, FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "projection"), 1, FALSE, glm::value_ptr(projection));
		models.draw(shader.get_id());

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();

	return 0;
}



