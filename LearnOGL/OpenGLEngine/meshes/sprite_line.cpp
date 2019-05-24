#include "sprite_line.h"


CGLLine::CGLLine()
{
	_VAO = 0;
	_VBO = 0;
}

CGLLine::~CGLLine()
{
}

bool CGLLine::init()
{
	GLfloat vertices[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return true;
}

void CGLLine::uninit()
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

bool CGLLine::draw()
{
	if (_VAO > 0) {
		glBindVertexArray(_VAO);
		glDrawArrays(GL_LINES, 0, 2);
		return true;
	}

	return false;
}

