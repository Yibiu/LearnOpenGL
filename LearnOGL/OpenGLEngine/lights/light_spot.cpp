#include "light_spot.h"


CGLSpotLight::CGLSpotLight()
{
}

CGLSpotLight::~CGLSpotLight()
{
}

void CGLSpotLight::set_position(const glm::vec3 &pos)
{
	_spot.position = pos;
}

void CGLSpotLight::set_direction(const glm::vec3 &dir)
{
	_spot.direction = dir;
}

glm::vec3 CGLSpotLight::get_position()
{
	return _spot.position;
}

glm::vec3 CGLSpotLight::get_direction()
{
	return _spot.direction;
}

void CGLSpotLight::set_constant(float constant)
{
	_spot.constant = constant;
}

void CGLSpotLight::set_linear(float linear)
{
	_spot.linear = linear;
}

void CGLSpotLight::set_quadratic(float quadratic)
{
	_spot.quadratic = quadratic;
}

float CGLSpotLight::get_constant()
{
	return _spot.constant;
}

float CGLSpotLight::get_linear()
{
	return _spot.linear;
}

float CGLSpotLight::get_quadratic()
{
	return _spot.quadratic;
}

void CGLSpotLight::set_cutoff(float cutoff)
{
	_spot.cutoff = cutoff;
}

void CGLSpotLight::set_outer_cutoff(float outer_cutoff)
{
	_spot.outer_cutoff = outer_cutoff;
}

float CGLSpotLight::get_cutoff()
{
	return _spot.cutoff;
}

float CGLSpotLight::get_outer_cutoff()
{
	return _spot.outer_cutoff;
}

