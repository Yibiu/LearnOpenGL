#include "camera.h"


CGLCamera::CGLCamera(uint32_t win_width, uint32_t win_height,
	glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
	_win_width = win_width;
	_win_height = win_height;

	_camera_pos = pos;
	_camera_front = front;
	_camera_up = up;

	_yaw = -90.0f;
	_pitch = 0.0f;

	_first_mouse = true;
	_last_mouse_x = 0.0f;
	_last_mouse_y = 0.0f;
	_fov = 45.0f;
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
	float speed = MOVING_SPEED_FACTOR * dt;
	switch (type)
	{
	case MOVING_LEFT:
		_camera_pos -= (speed * glm::normalize(glm::cross(_camera_front, _camera_up)));
		break;
	case MOVING_RIGHT:
		_camera_pos += (speed * glm::normalize(glm::cross(_camera_front, _camera_up)));
		break;
	case MOVING_FRONT:
		_camera_pos += (speed * _camera_front);
		break;
	case MOVING_BACK:
		_camera_pos -= (speed * _camera_front);
		break;
	default:
		break;
	}
}

void CGLCamera::update_rotating(float xpos, float ypos)
{
	if (_first_mouse) {
		_last_mouse_x = xpos;
		_last_mouse_y = ypos;
		_first_mouse = false;
	}

	float offset_x = xpos - _last_mouse_x;
	float offset_y = ypos - _last_mouse_y;
	_last_mouse_x = xpos;
	_last_mouse_y = ypos;

	// x, y ----> yaw, pitch
	offset_x *= ROTATING_SPEED_FACTOR;
	offset_y *= ROTATING_SPEED_FACTOR;
	_yaw += offset_x;
	_pitch -= offset_y;
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;
	glm::vec3 front(1.0f);
	front.x = cos(glm::radians(_yaw));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	// yaw, pitch ----> camera_front
	_camera_front = glm::normalize(front);
}

void CGLCamera::update_scrolling(float xoffset, float yoffset)
{
	if (_fov >= 1.0f && _fov <= 45.0f) {
		_fov -= yoffset;
	}
	if (_fov <= 1.0f) {
		_fov = 1.0f;
	}
	if (_fov >= 45.0f) {
		_fov = 45.0f;
	}
}

glm::mat4 CGLCamera::get_view()
{
	return glm::lookAt(_camera_pos, _camera_pos + _camera_front, _camera_up);
}

glm::mat4 CGLCamera::get_perspective()
{
	return glm::perspective(glm::radians(_fov), (float)_win_width / (float)_win_height, 0.1f, 100.0f);
}

