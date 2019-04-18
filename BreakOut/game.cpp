#include "game.h"


// CGameLevel
CGameLevel::CGameLevel()
{
	_sprites_datas.clear();
}

CGameLevel::~CGameLevel()
{
}

bool CGameLevel::init(const GLchar *cfg_ptr)
{
	bool error = false;

	_sprites_datas.clear();
	std::ifstream in(cfg_ptr);
	do {
		if (!in.is_open()) {
			error = true;
			break;
		}

		uint32_t col = 0;
		std::vector<std::vector<uint32_t>> sprites_desc;
		std::string line;
		while (getline(in, line))
		{
			std::vector<uint32_t> descs;
			_parse_line(line, descs);
			if (0 == descs.size()) {
				error = true;
				break;
			}
			if (0 == col) {
				col = descs.size();
			}
			else if (descs.size() != col){
				error = true;
				break;
			}
			sprites_desc.push_back(descs);
		}
		if (0 == sprites_desc.size()) {
			error = true;
		}
		if (error)
			break;

		_generate_sprites(sprites_desc);
		_sprite.init();
	} while (false);
	in.close();

	if (error) {
		uinit();
	}

	return (!error);
}

void CGameLevel::uinit()
{
	_sprites_datas.clear();
	_sprite.uninit();
}

void CGameLevel::update(const sprite_data_t &ball)
{
	uint32_t row = _sprites_datas.size();
	uint32_t col = _sprites_datas[0].size();
	for (uint32_t i = 0; i < row; i++) {
		for (uint32_t j = 0; j < col; j++) {
			if (!_sprites_datas[i][j].hidden && is_collision(ball, _sprites_datas[i][j])) {
				_sprites_datas[i][j].hidden = true;
			}
		}
	}
}

void CGameLevel::draw(CNGLShader &shader, CNGLTexture &texture)
{
	uint32_t row = _sprites_datas.size();
	uint32_t col = _sprites_datas[0].size();
	for (uint32_t i = 0; i < row; i++) {
		for (uint32_t j = 0; j < col; j++) {
			if (!_sprites_datas[i][j].hidden) {
				_sprite.draw(shader, texture, _sprites_datas[i][j]);
			}
		}
	}
}


void CGameLevel::_parse_line(std::string str, std::vector<uint32_t> &descs)
{
	descs.clear();
	if (str.empty())
		return;

	size_t pos = str.find(",");
	while (pos != std::string::npos)
	{
		descs.push_back(std::stoi(str.substr(0, pos)));
		str = str.substr(pos + 1);
		pos = str.find(",");
	}
	descs.push_back(std::stoi(str));
}

void CGameLevel::_generate_sprites(const std::vector<std::vector<uint32_t>> &descs)
{
	uint32_t row = descs.size();
	uint32_t col = descs[0].size();
	uint32_t width = WINDOW_WIDTH / col;
	uint32_t height = WINDOW_HEIGHT / (2 * row);
	_sprites_datas.clear();
	for (uint32_t i = 0; i < row; i++) {
		std::vector<sprite_data_t> datas;
		for (uint32_t j = 0; j < col; j++) {
			sprite_data_t data;
			data.hidden = false;
			data.left = j * width;
			data.top = i * height;
			data.width = width;
			data.height = height;
			data.rotate = 0.f;
			switch (descs[i][j])
			{
			case SPRITE_COLOR_NONE:
				data.hidden = true;
				break;
			case SPRITE_COLOR_ROCK:
				data.color = glm::vec3(0.9f, 0.1f, 0.9f);
				break;
			case SPRITE_COLOR_ORANGE:
				data.color = glm::vec3(1.f, 0.8f, 0.5f);
				break;
			case SPRITE_COLOR_YELLOW:
				data.color = glm::vec3(0.2f, 0.6f, 0.4f);
				break;
			case SPRITE_COLOR_BLUE:
				data.color = glm::vec3(0.4f, 0.6f, 0.8f);
				break;
			case SPRITE_COLOR_GREEN:
				data.color = glm::vec3(0.4f, 0.8f, 0.6f);
				break;
			default:
				data.hidden = true;
				break;
			}
			datas.push_back(data);
		}
		_sprites_datas.push_back(datas);
	}
}


// CGame
CGame::CGame()
{
	_width = 0;
	_height = 0;
	memset(_key_masks, KEY_NONE, sizeof(_key_masks));

	_x_speed = X_SPEED_FACTOR;
	_y_speed = -Y_SPEED_FACTOR;
}

CGame::~CGame()
{
}

void CGame::init(GLuint width, GLuint height)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	_sprite_shader.init("./Resources/shaders/sprite.vs","./Resources/shaders/sprite.fs");
	_sprite_shader.use();
	_sprite_shader.set_int("image", 0);
	_sprite_shader.set_mat4("projection", projection);
	_sprite_texture.init("./Resources/images/block.png");
	_plate_texture.init("./Resources/images/paddle.png");
	_plate_data.hidden = false;
	_plate_data.left = 200;
	_plate_data.top = WINDOW_HEIGHT - 20;
	_plate_data.width = 100;
	_plate_data.height = 20;
	_plate_data.rotate = 0.f;
	_plate_data.color = glm::vec3(1.f, 1.f, 1.f);
	_ball_texture.init("./Resources/images/awesomeface.png");
	_ball_data.hidden = false;
	_ball_data.left = 250;
	_ball_data.top = WINDOW_HEIGHT - 40;
	_ball_data.width = 20;
	_ball_data.height = 20;
	_ball_data.rotate = 0.f;
	_ball_data.color = glm::vec3(1.f, 1.f, 1.f);
	_sprite.init();
	
	_level.init("./Resources/levels/level1.dat");
}

void CGame::uninit()
{
	_sprite_shader.unint();
	_sprite_texture.uninit();
	_plate_texture.uninit();
	_ball_texture.uninit();
	_sprite.uninit();

	_level.uinit();
}

void CGame::set_key(uint32_t key, key_state_t state)
{
	_key_masks[key] = state;
}

void CGame::update(GLfloat dt)
{
	// Update ball position
	_ball_data.left +=  _x_speed * dt;
	_ball_data.top += _y_speed * dt;
	if (_ball_data.left < 0) { // Left border
		_ball_data.left = 0;
		_x_speed = -_x_speed;
	}
	if (_ball_data.left + _ball_data.width > WINDOW_WIDTH) { // Right border
		_ball_data.left = WINDOW_WIDTH - _ball_data.width;
		_x_speed = -_x_speed;
	}
	if (_ball_data.top < 0) { // Top border
		_ball_data.top = 0;
		_y_speed = -_y_speed;
	}

	// Update rocks
	_level.update(_ball_data);

	// Update plate position
	if (_key_masks[65] == KEY_PRESSED) { // A
		_plate_data.left -= X_SPEED_FACTOR * dt;
	}
	if (_key_masks[68] == KEY_PRESSED) { // D
		_plate_data.left += X_SPEED_FACTOR * dt;
	}
	if (_plate_data.left < 0) { // Left border
		_plate_data.left = 0;
	}
	if (_plate_data.left + _plate_data.width > WINDOW_WIDTH) {
		_plate_data.left = WINDOW_WIDTH - _plate_data.width;
	}

	// Collision: ball and plate
	if (is_collision(_ball_data, _plate_data)) {
		_y_speed = -_y_speed;
	}
}

void CGame::render()
{
	_level.draw(_sprite_shader, _sprite_texture);
	_sprite.draw(_sprite_shader, _plate_texture, _plate_data);
	_sprite.draw(_sprite_shader, _ball_texture, _ball_data);
}


