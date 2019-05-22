#pragma once
#include "../common/defines.h"


/**
* @brief:
* Basic class for all OpenGL resources and objects
*/
class CGLObject
{
public:
	typedef enum _obj_type
	{
		OBJ_UNKNOWN = 0x00,
		OBJ_SHADER,
		OBJ_TEXTURE,
		OBJ_MESH
	} obj_type_t;

public:
	CGLObject();
	virtual ~CGLObject();

	obj_type_t get_type();
	void set_name(const std::string &name);
	std::string get_name();

protected:
	obj_type_t _type;
	std::string _name;
};
