#include "sprite_circle.h"


CGLCircle::CGLCircle()
{
	_VAO = 0;
	_VBO = 0;

	_vertices.clear();
}

CGLCircle::~CGLCircle()
{
}

bool CGLCircle::init()
{
	float r = 0.5;
	uint32_t split = 120;
	float split_rad = 360.0f / split;

	// Circle
	_vertices.clear();
	_vertices.push_back(0.0f);
	_vertices.push_back(0.0f);
	_vertices.push_back(0.0f);
	for (int i = 0; i <= split; i++) {
		_vertices.push_back(r * cos(glm::radians(i * split_rad)));
		_vertices.push_back(0.0f);
		_vertices.push_back(-r * sin(glm::radians(i * split_rad)));
	}
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void CGLCircle::uninit()
{
	if (_VBO > 0) {
		glDeleteBuffers(1, &_VBO);
		_VBO = 0;
	}
	if (_VAO > 0) {
		glDeleteVertexArrays(1, &_VAO);
		_VAO = 0;
	}
	_vertices.clear();
}

bool CGLCircle::draw()
{
	if (_VAO > 0) {
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices.size() / 3);
		return true;
	}

	return false;
}

