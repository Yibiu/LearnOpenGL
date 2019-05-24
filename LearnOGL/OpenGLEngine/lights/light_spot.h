#pragma once
#include "light.h"


/**
* @brief:
* Spot light
*/
class CGLSpotLight : public CGLLight
{
	typedef struct _spot_data
	{
		glm::vec3 position;
		glm::vec3 direction;

		float constant;
		float linear;
		float quadratic;

		float cutoff;
		float outer_cutoff;
	} spot_data_t;

public:
	CGLSpotLight();
	virtual ~CGLSpotLight();

	void set_position(const glm::vec3 &pos);
	void set_direction(const glm::vec3 &dir);
	glm::vec3 get_position();
	glm::vec3 get_direction();

	void set_constant(float constant);
	void set_linear(float linear);
	void set_quadratic(float quadratic);
	float get_constant();
	float get_linear();
	float get_quadratic();

	void set_cutoff(float cutoff);
	void set_outer_cutoff(float outer_cutoff);
	float get_cutoff();
	float get_outer_cutoff();

protected:
	spot_data_t _spot;
};

