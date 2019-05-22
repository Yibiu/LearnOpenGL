#pragma once
#include "object.h"


/**
* @brief:
* Basic class for all sprites
*/
class CGLSprite : public CGLObject
{
public:
	typedef struct _sprite_data
	{
		glm::mat4 model;
	} sprite_data_t;

public:
	CGLSprite();
	virtual ~CGLSprite();

	virtual bool init() = 0;
	virtual void uninit() = 0;
	virtual bool draw() = 0;

	void set_model(const glm::mat4 &model);
	glm::mat4 get_model();

protected:
	sprite_data_t _data;
};

