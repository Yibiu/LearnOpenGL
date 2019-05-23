#include "lights.h"


CGLLight::CGLLight(glm::vec3 light_pos)
{
	_data.position = light_pos;
}

CGLLight::~CGLLight()
{
}

void CGLLight::set_position(const glm::vec3 &pos)
{
	_data.position = pos;
}

glm::vec3 CGLLight::get_position()
{
	return _data.position;
}

void CGLLight::set_ambient(const glm::vec3 &ambient)
{
	_data.ambient = ambient;
}

void CGLLight::set_diffuse(const glm::vec3 &diffuse)
{
	_data.diffuse = diffuse;
}

void CGLLight::set_specular(const glm::vec3 &specular)
{
	_data.specular = specular;
}

glm::vec3 CGLLight::get_ambient()
{
	return _data.ambient;
}

glm::vec3 CGLLight::get_diffuse()
{
	return _data.diffuse;
}

glm::vec3 CGLLight::get_specular()
{
	return _data.specular;
}

