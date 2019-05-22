#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Triangle
*
* - Gray triangle(no shader)
* - Color triangle(with shader)
*/
class CGLTriangle : public ICGLSprite
{
public:
	CGLTriangle();
	virtual ~CGLTriangle();

	// ICGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw(sprite_data_t *data);

protected:
	GLuint _VAO;
	GLuint _VBO;
};


