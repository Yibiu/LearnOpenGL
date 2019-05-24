#pragma once
#include <vector>
#include "sprite.h"


/**
* @brief:
*
*/
class CGLCylinder : public CGLSprite
{
public:
	CGLCylinder();
	virtual ~CGLCylinder();

	// CGLSprite
	virtual bool init();
	virtual void uninit();
	virtual bool draw();

protected:
	GLuint _VAO_top;
	GLuint _VBO_top;
	GLuint _VAO_side;
	GLuint _VBO_side;
	GLuint _VAO_bottom;
	GLuint _VBO_bottom;

	std::vector<GLfloat> _vertices_top;
	std::vector<GLfloat> _vertices_side;
	std::vector<GLfloat> _vertices_bottom;
};

