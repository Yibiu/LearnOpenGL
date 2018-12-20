#include "game.h"


CNGLGame::CNGLGame()
{
	_width = 0;
	_height = 0;
	memset(_key_masks, KEY_NONE, sizeof(_key_masks));
}

CNGLGame::~CNGLGame()
{
}

void CNGLGame::init(GLuint width, GLuint height)
{
	_shader.init("./sprite.vs","./sprite.fs");
	_shader.use();
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	_shader.set_int("image", 0);
	_shader.set_mat4("projection", projection);
	_texture.init("./awesomeface.png");
	_sprite.init();
}

void CNGLGame::uninit()
{
	_shader.unint();
	_texture.uninit();
	_sprite.uninit();
}

void CNGLGame::set_key(uint32_t key, key_state_t state)
{
	_key_masks[key] = state;
}

void CNGLGame::update(GLfloat dt)
{

}

void CNGLGame::render()
{
	sprite_date_t data;
	data.left = 0;
	data.top = 0;
	data.width = 200;
	data.height = 200;
	data.rotate = 0.0f;
	data.color = glm::vec3(0.0, 1.0, 0.0);
	_sprite.draw(_shader, _texture, data);
}

