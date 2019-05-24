#pragma once
#include <vector>
#include "sprite.h"


/**
* @brief:
* Sprite - Conus
*/
class CGLConus : public CGLSprite
{
public:
	CGLConus();
	virtual ~CGLConus();

	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO_side;
	GLuint _VBO_side;
	GLuint _VAO_bottom;
	GLuint _VBO_bottom;

	std::vector<GLfloat> _vertices_side;
	std::vector<GLfloat> _vertices_bottom;
};

