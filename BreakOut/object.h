#pragma once
#include <stdint.h>
#include <string>
using namespace std;


/**
* @brief:
* Basic class for opengl objects and resources.
*/
class CNGLObject
{
public:
	typedef enum _obj_type
	{
		OBJ_UNKNOWN = 0,
		OBJ_SHADER,
		OBJ_TEXTURE,
		OBJ_MESH
	} obj_type_t;

public:
	CNGLObject();
	virtual ~CNGLObject();

	obj_type_t get_type();
	void set_name(const string &name);
	string get_name();

protected:
	obj_type_t _type;
	string _name;
};

