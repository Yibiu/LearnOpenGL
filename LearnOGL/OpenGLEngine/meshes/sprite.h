#pragma once
#include "object.h"


typedef struct _sprite_data
{
	rect_t position;
} sprite_data_t;


/**
* @brief:
* Basic class for all sprites
*/
class ICGLSprite : public CGLObject
{
public:
	ICGLSprite() {}
	virtual ~ICGLSprite() {}

	virtual bool init() = 0;
	virtual void uninit() = 0;
	virtual bool draw(sprite_data_t *data) = 0;
};

