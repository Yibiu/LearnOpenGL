#include "sprite_rect.h"


CGLRectangle::CGLRectangle()
{
	_VAO = 0;
	_VBO = 0;
	_IBO = 0;
}

CGLRectangle::~CGLRectangle()
{
}

bool CGLRectangle::init()
{
	float vertices[] = {
		// positions       // texture coords
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f,	// Top-right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// Bottom-right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// Bottom-left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f	// Top-left
	};
	unsigned int indices[] = {
		0, 1, 3,	// Triangle 1
		1, 2, 3		// Triangle 2
	};
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void CGLRectangle::uninit()
{
	if (_IBO > 0) {
		glDeleteBuffers(1, &_IBO);
		_IBO = 0;
	}
	if (_VBO > 0) {
		glDeleteBuffers(1, &_VBO);
		_VBO = 0;
	}
	if (_VAO > 0) {
		glDeleteVertexArrays(1, &_VAO);
		_VAO = 0;
	}
}

bool CGLRectangle::draw(sprite_data_t *data)
{
	if (_VAO > 0) {
		glBindVertexArray(_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		return true;
	}

	return false;
}

