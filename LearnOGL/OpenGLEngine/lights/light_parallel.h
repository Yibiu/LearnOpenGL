#pragma once
#include "light.h"


/**
* @brief:
* Parallel light
*/
class CGLParallelLight : public CGLLight
{
	typedef struct _parallel_data {
		glm::vec3 direction;
	} parallel_data_t;

public:
	CGLParallelLight();
	virtual ~CGLParallelLight();

	void set_direction(const glm::vec3 &dir);
	glm::vec3 get_direction();

protected:
	parallel_data_t _parallel;
};

