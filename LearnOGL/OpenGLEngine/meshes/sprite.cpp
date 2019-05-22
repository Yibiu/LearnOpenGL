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

