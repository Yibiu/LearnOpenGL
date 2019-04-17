#include "game.h"


// CGameLevel
CGameLevel::CGameLevel()
{
}

CGameLevel::~CGameLevel()
{
}

void CGameLevel::init(const GLchar *cfg_ptr)
{
}

void CGameLevel::uinit()
{
}

void CGameLevel::draw(CNGLShader &shader, CNGLTexture &texture)
{
}


// CGame
CGame::CGame()
{
	_width = 0;
	_height = 0;
	memset(_key_masks, KEY_NONE, sizeof(_key_masks));
}

CGame::~CGame()
{
}

void CGame::init(GLuint width, GLuint height)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	_sprite_shader.init("./sprite.vs","./sprite.fs");
	_sprite_shader.use();
	_sprite_shader.set_int("image", 0);
	_sprite_shader.set_mat4("projection", projection);
	_sprite_texture.init("./awesomeface.png");
	_sprite.init();
}

void CGame::uninit()
{
	_sprite_shader.unint();
	_sprite_texture.uninit();
	_sprite.uninit();
}

void CGame::set_key(uint32_t key, key_state_t state)
{
	_key_masks[key] = state;
}

void CGame::update(GLfloat dt)
{
}

void CGame::render()
{
	sprite_date_t data;
	data.left = 0;
	data.top = 0;
	data.width = 200;
	data.height = 200;
	data.rotate = 0.0f;
	data.color = glm::vec3(0.0, 1.0, 0.0);
	_sprite.draw(_sprite_shader, _sprite_texture, data);
}

