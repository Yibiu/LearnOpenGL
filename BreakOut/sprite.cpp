#include "sprite.h"


CSprite::CSprite()
{
	_VAO = 0;
}

CSprite::~CSprite()
{
}

void CSprite::init()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CSprite::uninit()
{
	glDeleteVertexArrays(1, &_VAO);
}

void CSprite::draw(CNGLShader &shader, CNGLTexture &texture, sprite_date_t &data)
{
	shader.use();
	texture.bind();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(data.left, data.top, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * data.width, 0.5f * data.height, 0.0f));
	model = glm::rotate(model, data.rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * data.width, -0.5f * data.height, 0.0f));
	model = glm::scale(model, glm::vec3(data.width, data.height, 1.0f));
	shader.set_mat4("model", model);
	shader.set_vec3f("spriteColor", data.color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

