#pragma once
#include "light.h"


/**
* @brief:
* Dot light
*/
class CGLDotLight : public CGLLight
{
	typedef struct _dot_data {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;
	} dot_data_t;

public:
	CGLDotLight();
	virtual ~CGLDotLight();

	void set_position(const glm::vec3 &pos);
	glm::vec3 get_position();

	void set_constant(float constant);
	void set_linear(float linear);
	void set_quadratic(float quadratic);
	float get_constant();
	float get_linear();
	float get_quadratic();

protected:
	dot_data_t _dot;
};

