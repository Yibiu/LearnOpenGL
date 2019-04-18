#pragma once
#include <memory>
#include <vector>
#include "sprite.h"


#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

#define X_SPEED_FACTOR	150
#define Y_SPEED_FACTOR	150

typedef enum _key_state
{
	KEY_NONE = 0,
	KEY_PRESSED
} key_state_t;

typedef enum _sprite_color
{
	SPRITE_COLOR_NONE = 0x00,
	SPRITE_COLOR_ROCK,
	SPRITE_COLOR_ORANGE,
	SPRITE_COLOR_YELLOW,
	SPRITE_COLOR_BLUE,
	SPRITE_COLOR_GREEN
} sprite_color_t;


static inline bool is_collision(const sprite_data_t &one, const sprite_data_t &two)
{
	bool x_collision = ((one.left + one.width >= two.left) && (one.left <= two.left + two.width));
	bool y_collision = ((one.top + one.height >= two.top) && (one.top <= two.top + two.height));
	return (x_collision && y_collision);
}


/**
* @brief:
* Total: 800x600
*
* Upper(800x300)
* ------
* Downer(800x300)
*/
class CGameLevel
{
public:
	CGameLevel();
	virtual ~CGameLevel();

	bool init(const GLchar *cfg_ptr);
	void uinit();
	void update(const sprite_data_t &ball);
	void draw(CNGLShader &shader, CNGLTexture &sprite_tex);

protected:
	void _parse_line(std::string str, std::vector<uint32_t> &descs);
	void _generate_sprites(const std::vector<std::vector<uint32_t>> &descs);

protected:
	std::vector<std::vector<sprite_data_t>> _sprites_datas;
	CSprite _sprite;
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
	CGameLevel _level;

	sprite_data_t _plate_data;
	CNGLTexture _plate_texture;
	sprite_data_t _ball_data;
	CNGLTexture _ball_texture;
	CSprite _sprite;

	GLint _x_speed;
	GLint _y_speed;
};

