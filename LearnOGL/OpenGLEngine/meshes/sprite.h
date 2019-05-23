#pragma once
#include "object.h"


/**
* @brief:
* Basic class for all sprites
*/
class CGLSprite : public CGLObject
{
public:
	typedef struct _sprite_data
	{
		glm::mat4 model;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	} sprite_data_t;

public:
	CGLSprite();
	virtual ~CGLSprite();

	virtual bool init() = 0;
	virtual void uninit() = 0;
	virtual bool draw() = 0;

	void set_model(const glm::mat4 &model);
	glm::mat4 get_model();

	void set_ambient(const glm::vec3 &ambient);
	void set_diffuse(const glm::vec3 &diffuse);
	void set_specular(const glm::vec3 &specular);
	void set_shininess(float shininess);
	glm::vec3 get_ambient();
	glm::vec3 get_diffuse();
	glm::vec3 get_specular();
	float get_shininess();

protected:
	sprite_data_t _data;
};

