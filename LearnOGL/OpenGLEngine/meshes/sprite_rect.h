#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Rectangle
*/
class CGLRectangle : public ICGLSprite
{
public:
	CGLRectangle();
	virtual ~CGLRectangle();

	// ICGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw(sprite_data_t *data);

protected:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _IBO;
};
