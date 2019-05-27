#pragma once
#include <vector>
#include "sprite.h"


/**
* @brief:
* Sprite - Ball
*/
class CGLBall : public CGLSprite
{
public:
	CGLBall();
	virtual ~CGLBall();

	// CGLSprite
	bool init();
	void uninit();
	bool draw();

protected:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _IBO;
	std::vector<GLfloat> _vertices;
	std::vector<GLuint> _indexes;
};

