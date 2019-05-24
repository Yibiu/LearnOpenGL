#include "light_dot.h"


CGLDotLight::CGLDotLight()
{
}

CGLDotLight::~CGLDotLight()
{
}

void CGLDotLight::set_position(const glm::vec3 &pos)
{
	_dot.position = pos;
}

glm::vec3 CGLDotLight::get_position()
{
	return _dot.position;
}

void CGLDotLight::set_constant(float constant)
{
	_dot.constant = constant;
}

void CGLDotLight::set_linear(float linear)
{
	_dot.linear = linear;
}

void CGLDotLight::set_quadratic(float quadratic)
{
	_dot.quadratic = quadratic;
}

float CGLDotLight::get_constant()
{
	return _dot.constant;
}

float CGLDotLight::get_linear()
{
	return _dot.linear;
}

float CGLDotLight::get_quadratic()
{
	return _dot.quadratic;
}
