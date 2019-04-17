#include "game.h"


// CGameLevel
CGameLevel::CGameLevel()
{
	_sprites.clear();
}

CGameLevel::~CGameLevel()
{
}

bool CGameLevel::init(const GLchar *cfg_ptr)
{
	std::ifstream in(cfg_ptr);
	if (!in.is_open())
		return false;

	uint32_t line_count = 0;
	std::string line;
	while (getline(in, line))
	{
		std::vector<uint32_t> elems;
		_parse_line(line, elems);

		sprite_date_t data;
	}

	in.close();
	return true;
}

void CGameLevel::uinit()
{
}

void CGameLevel::draw(CNGLShader &shader, CNGLTexture &texture)
{
}


void CGameLevel::_parse_line(std::string str, std::vector<uint32_t> &elems)
{
	elems.clear();
	if (str.empty())
		return;

	size_t pos = str.find(",");
	while (pos != std::string::npos)
	{
		elems.push_back(std::stoi(str.substr(0, pos)));
		str = str.substr(pos + 1);
		pos = str.find(",");
	}
	elems.push_back(std::stoi(str));
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

