#pragma once
#include "sprite.h"


/**
* @brief:
* Sprite - Cube
*/
class CGLCube : public CGLSprite
{
public:
	CGLCube();
	virtual ~CGLCube();

	// ICGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
};
