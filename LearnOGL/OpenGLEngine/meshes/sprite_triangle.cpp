#include "sprite_triangle.h"


CGLTriangle::CGLTriangle()
{
	_VAO = 0;
	_VBO = 0;
}

CGLTriangle::~CGLTriangle()
{
}

bool CGLTriangle::init()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,	// Left
		0.5f, -0.5f, 0.0f,	// Right
		0.0f,  0.5f, 0.0f	// Top
	};
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void CGLTriangle::uninit()
{
	if (_VBO > 0) {
		glDeleteBuffers(1, &_VBO);
		_VBO = 0;
	}
	if (_VAO > 0) {
		glDeleteVertexArrays(1, &_VAO);
		_VAO = 0;
	}
}

bool CGLTriangle::draw(sprite_data_t *data)
{
	if (_VAO > 0) {
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		return true;
	}

	return false;
}


