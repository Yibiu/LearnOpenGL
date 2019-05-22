#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Rectangle
*/
class CGLRectangle : public CGLSprite
{
public:
	CGLRectangle();
	virtual ~CGLRectangle();

	// ICGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _IBO;
};
