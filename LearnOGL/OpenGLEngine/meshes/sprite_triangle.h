#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Triangle
*
* - Gray triangle(no shader)
* - Color triangle(with shader)
*/
class CGLTriangle : public CGLSprite
{
public:
	CGLTriangle();
	virtual ~CGLTriangle();

	// ICGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
};


