#pragma once
#include <stdint.h>
#include "shader.h"
#include "texture.h"


typedef struct _sprite_date
{
	uint32_t top;
	uint32_t left;
	uint32_t width;
	uint32_t height;

	float rotate;
	glm::vec3 color;
} sprite_date_t;


class CSprite
{
public:
	CSprite();
	virtual ~CSprite();

	void init();
	void uninit();
	void draw(CNGLShader &shader, CNGLTexture &texture, sprite_date_t &data);

protected:
	GLuint _VAO;
};

