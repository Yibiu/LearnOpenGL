#include "sprite_conus.h"


CGLConus::CGLConus()
{
	_VAO_side = 0;
	_VBO_side = 0;
	_VAO_bottom = 0;
	_VBO_bottom = 0;

	_vertices_side.clear();
	_vertices_bottom.clear();
}

CGLConus::~CGLConus()
{
}

bool CGLConus::init()
{
	float r = 0.5;
	uint32_t split = 120;
	float split_rad = 360.0f / split;

	// Side
	_vertices_side.clear();
	_vertices_side.push_back(0.0f);
	_vertices_side.push_back(0.5f);
	_vertices_side.push_back(0.0f);
	for (int i = 0; i <= split; i++) {
		_vertices_side.push_back(r * cos(glm::radians(i * split_rad)));
		_vertices_side.push_back(-0.5f);
		_vertices_side.push_back(-r * sin(glm::radians(i * split_rad)));
	}
	glGenVertexArrays(1, &_VAO_side);
	glBindVertexArray(_VAO_side);
	glGenBuffers(1, &_VBO_side);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_side);
	glBufferData(GL_ARRAY_BUFFER, _vertices_side.size() * sizeof(float), &_vertices_side[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Bottom
	_vertices_bottom.clear();
	_vertices_bottom.push_back(0.0f);
	_vertices_bottom.push_back(-0.5f);
	_vertices_bottom.push_back(0.0f);
	for (int i = 0; i <= split; i++) {
		_vertices_bottom.push_back(r * cos(glm::radians(i * split_rad)));
		_vertices_bottom.push_back(-0.5f);
		_vertices_bottom.push_back(-r * sin(glm::radians(i * split_rad)));
	}
	glGenVertexArrays(1, &_VAO_bottom);
	glBindVertexArray(_VAO_bottom);
	glGenBuffers(1, &_VBO_bottom);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_bottom);
	glBufferData(GL_ARRAY_BUFFER, _vertices_bottom.size() * sizeof(float), &_vertices_bottom[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void CGLConus::uninit()
{
	if (_VBO_side > 0) {
		glDeleteBuffers(1, &_VBO_side);
		_VBO_side = 0;
	}
	if (_VAO_side > 0) {
		glDeleteVertexArrays(1, &_VAO_side);
		_VAO_side = 0;
	}
	_vertices_side.clear();

	if (_VBO_bottom > 0) {
		glDeleteBuffers(1, &_VBO_bottom);
		_VBO_bottom= 0;
	}
	if (_VAO_bottom > 0) {
		glDeleteVertexArrays(1, &_VAO_bottom);
		_VAO_bottom = 0;
	}
	_vertices_bottom.clear();
}

bool CGLConus::draw()
{
	if (_VAO_side > 0 && _VAO_bottom > 0) {
		glBindVertexArray(_VAO_side);
		glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices_side.size() / 3);

		glBindVertexArray(_VAO_bottom);
		glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices_bottom.size() / 3);

		return true;
	}

	return false;
}

