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
using namespace std;
/**
* @brief:
*	Basic lights of openGL.
*	
*	Phong Lighting Model = Ambient Lighting + Diffuse Lighting + Specular Lighting.
*
* @ref:
*	https://learnopengl-cn.github.io/02%20Lighting/01%20Colors/
*/


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


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
	CShader light_shader, obj_shader;
	light_shader.compile("./light.vs", "./light.fs");
	obj_shader.compile("./object.vs", "./object.fs");
	// Rect
	float vertices[] = {
		// Positions
		// Face1
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		// Face2
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// Face3
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		// Face4
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		// Face5
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		// Face6
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int VBO, light_VAO, obj_VAO;
	glGenVertexArrays(1, &light_VAO);
	glGenVertexArrays(1, &obj_VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(obj_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	// Main loop
	glm::vec3 cam_position = glm::vec3(1.0f, 0.8f, 4.0f);
	glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cam_front = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
	glm::vec3 light_position = glm::vec3(1.2f, 1.0f, 2.0f);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window_ptr))
	{
		process_input(window_ptr);

		// Draw renderer... 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::lookAt(cam_position, cam_position + cam_front, cam_up);
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model(1.0);
		model = glm::translate(model, light_position);
		model = glm::scale(model, glm::vec3(0.2f));
		light_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(light_shader.get_id(), "model"), 1, FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(light_shader.get_id(), "view"), 1, FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(light_shader.get_id(), "projection"), 1, FALSE, glm::value_ptr(projection));
		glBindVertexArray(light_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		obj_shader.use();
		glUniform3fv(glGetUniformLocation(obj_shader.get_id(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		glUniform3fv(glGetUniformLocation(obj_shader.get_id(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(obj_shader.get_id(), "lightPos"), 1, glm::value_ptr(light_position));
		glUniform3fv(glGetUniformLocation(obj_shader.get_id(), "viewPos"), 1, glm::value_ptr(cam_position));
		glUniformMatrix4fv(glGetUniformLocation(obj_shader.get_id(), "model"), 1, FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(obj_shader.get_id(), "view"), 1, FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(obj_shader.get_id(), "projection"), 1, FALSE, glm::value_ptr(projection));
		glBindVertexArray(obj_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &light_VAO);
	glDeleteVertexArrays(1, &obj_VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate
	glfwTerminate();

	return 0;
}



