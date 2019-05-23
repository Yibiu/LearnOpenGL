#pragma once
#include "../common/defines.h"


#define MOVING_SPEED_FACTOR		(2.5f)
#define ROTATING_SPEED_FACTOR	(0.1f)


/**
* @brief:
* Camera for OpenGL
*
* Right-handed coordination
*/
class CGLCamera
{
public:
	typedef enum _moving_type
	{
		MOVING_LEFT = 0x00,
		MOVING_RIGHT,
		MOVING_FRONT,
		MOVING_BACK
	} moving_type_t;

public:
	CGLCamera(uint32_t win_width, uint32_t win_height,
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	virtual ~CGLCamera();

	void set_position(const glm::vec3 &pos);
	void set_front(const glm::vec3 &front);
	void set_up(const glm::vec3 &up);
	glm::vec3 get_position();
	glm::vec3 get_front();
	glm::vec3 get_up();

	void update_moving(moving_type_t type, float dt);
	void update_rotating(float xpos, float ypos);
	void update_scrolling(float xoffset, float yoffset);

	glm::mat4 get_view();
	glm::mat4 get_perspective();

protected:
	uint32_t _win_width;
	uint32_t _win_height;

	glm::vec3 _camera_pos;
	glm::vec3 _camera_front;
	glm::vec3 _camera_up;

	float _yaw;
	float _pitch;

	bool _first_mouse;
	float _last_mouse_x;
	float _last_mouse_y;
	float _fov;
};

