#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Plane
*/
class CGLPlane : public CGLSprite
{
public:
	CGLPlane();
	virtual ~CGLPlane();

	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _IBO;
};

