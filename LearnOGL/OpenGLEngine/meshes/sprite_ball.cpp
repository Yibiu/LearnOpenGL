#include "sprite_ball.h"


CGLBall::CGLBall()
{
	_VAO = 0;
	_VBO = 0;
	_IBO = 0;
	_vertices.clear();
	_indexes.clear();
}

CGLBall::~CGLBall()
{
}

bool CGLBall::init()
{
	float r = 1.0f;
	uint32_t row = 61;
	uint32_t col = 60;
	float row_angle = 180 / (row - 1);
	float col_angle = 360 / col;

	_vertices.clear();
	for (uint32_t i = 0; i < row; i++) {
		for (uint32_t j = 0; j < col; j++) {
			_vertices.push_back(r * sin(glm::radians(i * row_angle)) * cos(glm::radians(j * col_angle)));
			_vertices.push_back(r * cos(glm::radians(i * row_angle)));
			_vertices.push_back(-r * sin(glm::radians(i * row_angle)) * sin(glm::radians(j * col_angle)));
		}
	}

	_indexes.clear();
	for (uint32_t i = 0; i < row - 1; i++) {
		for (uint32_t j = 0; j < col; j++) {
			// Triangle0
			_indexes.push_back(i * col + j);
			_indexes.push_back((i + 1) * col + j);
			_indexes.push_back(i * col + (j + 1) % col);
			// Triagnle1
			_indexes.push_back((i + 1) * col + j);
			_indexes.push_back((i + 1) * col + (j + 1) % col);
			_indexes.push_back(i * col + (j + 1) % col);
		}
	}

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexes.size() * sizeof(GLuint), &_indexes[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void CGLBall::uninit()
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

	_vertices.clear();
	_indexes.clear();
}

bool CGLBall::draw()
{
	if (_VAO > 0) {
		glBindVertexArray(_VAO);
		glDrawElements(GL_TRIANGLES, _indexes.size(), GL_UNSIGNED_INT, 0);
		return true;
	}

	return false;
}

