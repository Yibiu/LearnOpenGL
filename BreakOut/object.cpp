#include "object.h"


CNGLObject::CNGLObject()
{
	_type = OBJ_UNKNOWN;
	_name = "";
}

CNGLObject::~CNGLObject()
{
}

CNGLObject::obj_type_t CNGLObject::get_type()
{
	return _type;
}

void CNGLObject::set_name(const std::string &name)
{
	_name = name;
}

std::string CNGLObject::get_name()
{
	return _name;
}
