#include "sprite.h"


CGLSprite::CGLSprite()
{
	_data.model = glm::mat4(1.0f);
}

CGLSprite::~CGLSprite()
{
}

void CGLSprite::set_model(const glm::mat4 &model)
{
	_data.model = model;
}

glm::mat4 CGLSprite::get_model()
{
	return _data.model;
}

void CGLSprite::set_ambient(const glm::vec3 &ambient)
{
	_data.ambient = ambient;
}

void CGLSprite::set_diffuse(const glm::vec3 &diffuse)
{
	_data.diffuse = diffuse;
}

void CGLSprite::set_specular(const glm::vec3 &specular)
{
	_data.specular = specular;
}

void CGLSprite::set_shininess(float shininess)
{
	_data.shininess = shininess;
}

glm::vec3 CGLSprite::get_ambient()
{
	return _data.ambient;
}

glm::vec3 CGLSprite::get_diffuse()
{
	return _data.diffuse;
}

glm::vec3 CGLSprite::get_specular()
{
	return _data.specular;
}

float CGLSprite::get_shininess()
{
	return _data.shininess;
}

