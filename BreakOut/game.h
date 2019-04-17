#pragma once
#include <memory>
#include <vector>
#include "sprite.h"


typedef enum _key_state
{
	KEY_NONE = 0,
	KEY_PRESSED
} key_state_t;


class CGameLevel
{
public:
	CGameLevel();
	virtual ~CGameLevel();

	bool init(const GLchar *cfg_ptr);
	void uinit();
	void draw(CNGLShader &shader, CNGLTexture &texture);

protected:
	void _parse_line(std::string str, std::vector<uint32_t> &elems);

protected:
	std::vector<sprite_date_t> _sprites;
};


class CGame
{
public:
	CGame();
	virtual ~CGame();

	void init(GLuint width, GLuint height);
	void uninit();
	void set_key(uint32_t key, key_state_t state); // 0x11: pressed, 0x00: released
	void update(GLfloat dt);
	void render();

protected:
	GLuint _width;
	GLuint _height;
	key_state_t _key_masks[1024];

	CNGLShader _sprite_shader;
	CNGLTexture _sprite_texture;
	CSprite _sprite;
};

