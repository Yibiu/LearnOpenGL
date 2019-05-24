#pragma once
#include <vector>
#include "sprite.h"


/**
* @brief:
*
*/
class CGLCircle : public CGLSprite
{
public:
	CGLCircle();
	virtual ~CGLCircle();

	// CGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;

	std::vector<GLfloat> _vertices;
};

