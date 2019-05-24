#include "light_parallel.h"


CGLParallelLight::CGLParallelLight()
{
}

CGLParallelLight::~CGLParallelLight()
{
}

void CGLParallelLight::set_direction(const glm::vec3 &dir)
{
	_parallel.direction = dir;
}

glm::vec3 CGLParallelLight::get_direction()
{
	return _parallel.direction;
}

