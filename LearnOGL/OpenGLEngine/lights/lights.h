#pragma once
#include "../common/defines.h"


/**
* Light effect for OpenGL
*/
class CGLLight
{
public:
	typedef struct _light_data
	{
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} light_data_t;

public:
	CGLLight(glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~CGLLight();

	void set_position(const glm::vec3 &pos);
	glm::vec3 get_position();

	void set_ambient(const glm::vec3 &ambient);
	void set_diffuse(const glm::vec3 &diffuse);
	void set_specular(const glm::vec3 &specular);
	glm::vec3 get_ambient();
	glm::vec3 get_diffuse();
	glm::vec3 get_specular();

protected:
	light_data_t _data;
};


