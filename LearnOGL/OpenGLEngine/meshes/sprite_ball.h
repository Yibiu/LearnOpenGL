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
	std::vector<GLuint> _VAOs;
	std::vector<GLuint> _VBOs;
	std::vector<std::vector<GLfloat>> _vertices;
};

