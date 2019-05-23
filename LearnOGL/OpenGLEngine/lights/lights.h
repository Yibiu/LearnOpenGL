#pragma once
#include "../common/defines.h"


/**
* Lights effect for OpenGL
*/
class CGLLights
{
public:
	CGLLights(glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~CGLLights();

	void set_light_position(const glm::vec3 &light_pos);
	void set_light_color(const glm::vec3 &light_color);
	glm::vec3 get_light_position();
	glm::vec3 get_light_color();

protected:
	glm::vec3 _light_pos;
	glm::vec3 _light_color;
};


