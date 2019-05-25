#include "sprite_ball.h"


CGLBall::CGLBall()
{
	_VAOs.clear();
	_VBOs.clear();
	_vertices.clear();
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

	std::vector<std::vector<coord_3df_t>> vertices;
	for (uint32_t i = 0; i < row; i++) {
		std::vector<coord_3df_t> row_vertices;
		for (uint32_t j = 0; j <= col; j++) {
			coord_3df_t coord;
			coord.x = r * sin(glm::radians(i * row_angle)) * cos(glm::radians(j * col_angle));
			coord.y = r * cos(glm::radians(i * row_angle));
			coord.z = -r * sin(glm::radians(i * row_angle)) * sin(glm::radians(j * col_angle));

			row_vertices.push_back(coord);
		}
		vertices.push_back(row_vertices);
	}

	_VAOs.clear();
	_VBOs.clear();
	_vertices.clear();
	for (uint32_t i = 0; i < row - 1; i++) {
		GLuint VAO;
		GLuint VBO;
		std::vector<GLfloat> row_vertices;
		for (uint32_t j = 0; j <= col; j++) {
			row_vertices.push_back(vertices[i][j].x);
			row_vertices.push_back(vertices[i][j].y);
			row_vertices.push_back(vertices[i][j].z);

			row_vertices.push_back(vertices[i + 1][j].x);
			row_vertices.push_back(vertices[i + 1][j].y);
			row_vertices.push_back(vertices[i + 1][j].z);
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, row_vertices.size() * sizeof(float), &row_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_VAOs.push_back(VAO);
		_VBOs.push_back(VBO);
		_vertices.push_back(row_vertices);
	}

	return true;
}

void CGLBall::uninit()
{
	for (int i = 0; i < _VBOs.size(); i++) {
		glDeleteBuffers(1, &_VBOs[i]);
	}
	_VBOs.clear();

	for (int i = 0; i < _VAOs.size(); i++) {
		glDeleteVertexArrays(1, &_VAOs[i]);
	}
	_VAOs.clear();

	_vertices.clear();
}

bool CGLBall::draw()
{
	if (_vertices.size() > 0 && _vertices.size() == _VAOs.size()) {
		for (int i = 0; i < _vertices.size(); i++) {
			glBindVertexArray(_VAOs[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices[i].size() / 3);
		}
		return true;
	}

	return false;
}

