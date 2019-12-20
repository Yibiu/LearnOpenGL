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
#include "assimp_parser.h"
using namespace std;
/**
* @brief:
* Mesh.
* Load models.
*
* @ref:
*	https://learnopengl-cn.github.io/03%20Model%20Loading/01%20Assimp/
*	https://learnopengl-cn.github.io/03%20Model%20Loading/02%20Mesh/
*	https://learnopengl-cn.github.io/03%20Model%20Loading/03%20Model/
*/


#define MAX_BONES_SLOTS		4
#define MAX_BONES_SUPPORT	100

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

	float speed = 8.f * delta_time;
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

	//////////////////////////////////////////////////////
	// Create resources
	// Shader
	//CShader shader;
	//shader.compile("./model.vs", "./model.fs");
	// Model
	//CNGLModel models;
	//models.load("./nanosuit/nanosuit.obj");

	CShader shader;
	shader.compile("./anim.vs", "./anim.fs");
	CAssimpParser parser;
	bool ret = parser.load(
		/*"./Content/boblampclean.md5mesh"*/
		/*"F:/IDE/Branch/DeferredShading/Engine/Assets/Models/Miku/miku.fbx"*/
		"C:/Users/Administrator/Desktop/Shoved Reaction With Spin.fbx"
	);
	if (!ret) {
		cout << "Assimp parse failed" << endl;
		return -1;
	}
	if (parser.get_scene().bones.size() > MAX_BONES_SUPPORT) {
		cout << "Bones out of support" << endl;
		return -1;
	}

	GLuint VAO;
	GLuint VBOs[5];
	enum VB_TYPES
	{
		VB_POS,
		VB_NORMAL,
		VB_TEXCOORD,
		VB_BONE,
		VB_INDEX
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(5, VBOs);

	// Position - atrribute 0
	std::vector<glm::vec3> positions;
	for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
		parser.get_scene().meshes[i].base_vertices = positions.size();
		for (uint32_t j = 0; j < parser.get_scene().meshes[i].vertices.size(); j++) {
			positions.push_back(parser.get_scene().meshes[i].vertices[j].position);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[VB_POS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Normal - attribute 1
	std::vector<glm::vec3> normals;
	for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
		for (uint32_t j = 0; j < parser.get_scene().meshes[i].vertices.size(); j++) {
			normals.push_back(parser.get_scene().meshes[i].vertices[j].normal);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[VB_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// TexCoord - attribute 2
	std::vector<glm::vec2> texcoords;
	for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
		for (uint32_t j = 0; j < parser.get_scene().meshes[i].vertices.size(); j++) {
			texcoords.push_back(parser.get_scene().meshes[i].vertices[j].texcoord);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[VB_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// Bone - attribute 3/4
	typedef struct _bone_buffer
	{
		uint32_t bone_idx[MAX_BONES_SLOTS];
		float bone_weight[MAX_BONES_SLOTS];
	} bone_buffer_t;
	std::vector<bone_buffer_t> bones;
	for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
		for (uint32_t j = 0; j < parser.get_scene().meshes[i].vertices.size(); j++) {
			bone_buffer_t buffer;

			// Init all map weights to 0.f
			for (uint32_t k = 0; k < MAX_BONES_SLOTS; k++) {
				buffer.bone_idx[k] = 0;
				buffer.bone_weight[k] = 0.f;
			}

			// Update some map weights
			uint32_t idx = 0;
			for (uint32_t k = 0; k < parser.get_scene().meshes[i].vertices[j].weights1.size(); k++) {
				uint32_t bone_idx = parser.get_scene().meshes[i].vertices[j].weights1[k].bone_idx;
				float bone_weight = parser.get_scene().meshes[i].vertices[j].weights1[k].bone_weight;

				if (idx >= MAX_BONES_SLOTS)
					break;
				buffer.bone_idx[idx] = bone_idx;
				buffer.bone_weight[idx] = bone_weight;
				++idx;
			}
			bones.push_back(buffer);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[VB_BONE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, MAX_BONES_SLOTS, GL_INT, sizeof(bone_buffer_t), (const GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, MAX_BONES_SLOTS, GL_FLOAT, GL_FALSE, sizeof(bone_buffer_t), (const GLvoid*)16);
	
	// Index
	std::vector<uint32_t> indices;
	for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
		parser.get_scene().meshes[i].base_indices = indices.size();
		indices.insert(indices.end(), parser.get_scene().meshes[i].indices.begin(),
			parser.get_scene().meshes[i].indices.end());
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[VB_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Texture (Only support diffuse now)
	std::map<std::string, GLuint> textures;
	for (uint32_t i = 0; i < parser.get_scene().materials.size(); i++) {
		if (parser.get_scene().materials[i].diffuses.size() > 0) {
			std::map<std::string, GLuint>::iterator iter = textures.find(parser.get_scene().materials[i].diffuses[0].path);
			if (iter == textures.end()) {
				GLuint TEX;
				glGenTextures(1, &TEX);
				glBindTexture(GL_TEXTURE_2D, TEX);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				int width, height, channels;
				//stbi_set_flip_vertically_on_load(true);
				uint8_t *data_ptr = stbi_load(parser.get_scene().materials[i].diffuses[0].path.c_str(), &width, &height, &channels, 0);
				if (NULL != data_ptr) {
					if (4 == channels) {
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_ptr);
					}
					else {
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_ptr);
					}
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else {
					cout << "Failed to load texture - " << parser.get_scene().materials[i].diffuses[0].path << endl;
					return -1;
				}
				stbi_image_free(data_ptr);

				textures[parser.get_scene().materials[i].diffuses[0].path] = TEX;
			}
		}
	}

	glBindVertexArray(0);
	//////////////////////////////////////////////////////

	// Main loop
	float start_sec = glfwGetTime();
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
		//models.draw(shader.get_id());

		// Bones transform
		std::vector<glm::mat4> trans;
		float time_sec = glfwGetTime() - start_sec;
		parser.transform(time_sec, trans);
		for (uint32_t i = 0; i < trans.size(); i++) {
			std::string str = "bones[" + std::to_string(i) + "]";
			glm::mat4 tran = glm::transpose(trans[i]);
			glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), str.c_str()), 1, GL_TRUE, (const GLfloat*)glm::value_ptr(tran));
		}

		glBindVertexArray(VAO);
		for (uint32_t i = 0; i < parser.get_scene().meshes.size(); i++) {
			// Textures
			//if (animation.get_scene().meshes[i].material_idx >= animation.get_scene().materials.size())
			//	continue;
			//
			// TODO ...

			if (parser.get_scene().materials[parser.get_scene().meshes[i].material_idx].diffuses.size() > 0) {
				glActiveTexture(GL_TEXTURE0);
				glUniform1i(glGetUniformLocation(shader.get_id(), "texture_diffuse1"), 0);
				glBindTexture(GL_TEXTURE_2D, textures[parser.get_scene().materials[parser.get_scene().meshes[i].material_idx].diffuses[0].path]);
			}

			glDrawElementsBaseVertex(GL_TRIANGLES,
				parser.get_scene().meshes[i].indices.size(),
				GL_UNSIGNED_INT,
				(void*)(sizeof(uint32_t) * parser.get_scene().meshes[i].base_indices),
				parser.get_scene().meshes[i].base_vertices);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();

	return 0;
}



