#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Line
*/
class CGLLine : public CGLSprite
{
public:
	CGLLine();
	virtual ~CGLLine();

	// CGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
};

