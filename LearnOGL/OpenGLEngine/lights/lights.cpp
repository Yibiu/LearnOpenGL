#include "lights.h"


CGLLights::CGLLights(glm::vec3 light_pos, glm::vec3 light_color)
{
	_light_pos = light_pos;
	_light_color = light_color;
}

CGLLights::~CGLLights()
{
}

void CGLLights::set_light_position(const glm::vec3 &light_pos)
{
	_light_pos = light_pos;
}

void CGLLights::set_light_color(const glm::vec3 &light_color)
{
	_light_color = light_color;
}

glm::vec3 CGLLights::get_light_position()
{
	return _light_pos;
}

glm::vec3 CGLLights::get_light_color()
{
	return _light_color;
}

