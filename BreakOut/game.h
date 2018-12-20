#pragma once
#include <memory>
#include "sprite.h"


typedef enum _key_state
{
	KEY_NONE = 0,
	KEY_PRESSED
} key_state_t;


class CNGLGame
{
public:
	CNGLGame();
	virtual ~CNGLGame();

	void init(GLuint width, GLuint height);
	void uninit();
	void set_key(uint32_t key, key_state_t state); // 0x11: pressed, 0x00: released
	void update(GLfloat dt);
	void render();

protected:
	GLuint _width;
	GLuint _height;
	key_state_t _key_masks[1024];

	CNGLShader _shader;
	CNGLTexture _texture;
	CSprite _sprite;
};

