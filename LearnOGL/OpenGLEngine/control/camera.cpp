#include "camera.h"


CGLCamera::CGLCamera(uint32_t win_width, uint32_t win_height,
	glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
	_win_width = win_width;
	_win_height = win_height;

	_camera_pos = pos;
	_camera_front = front;
	_camera_up = up;
}

CGLCamera::~CGLCamera()
{
}

void CGLCamera::set_position(const glm::vec3 &pos)
{
	_camera_pos = pos;
}

void CGLCamera::set_front(const glm::vec3 &front)
{
	_camera_front = front;
}

void CGLCamera::set_up(const glm::vec3 &up)
{
	_camera_up = up;
}

void CGLCamera::update_moving(moving_type_t type, float dt)
{
}

void CGLCamera::update_rotating(float xpos, float ypos)
{
}

void CGLCamera::update_scrolling(float xoffset, float yoffset)
{
}

glm::mat4 CGLCamera::get_view()
{
	return glm::lookAt(_camera_pos, _camera_pos + _camera_front, _camera_up);
}

glm::mat4 CGLCamera::get_perspective()
{
	return glm::perspective(glm::radians(_fov), (float)_win_width / (float)_win_height, 0.1f, 100.0f);
}

