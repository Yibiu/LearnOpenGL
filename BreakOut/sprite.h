#pragma once
#include <stdint.h>
#include "shader.h"
#include "texture.h"


typedef struct _sprite_data
{
	bool hidden;
	int top;
	int left;
	int width;
	int height;

	float rotate;
	glm::vec3 color;
} sprite_data_t;


class CSprite
{
public:
	CSprite();
	virtual ~CSprite();

	void init();
	void uninit();
	void draw(CNGLShader &shader, CNGLTexture &texture, sprite_data_t &data);

protected:
	GLuint _VAO;
};

