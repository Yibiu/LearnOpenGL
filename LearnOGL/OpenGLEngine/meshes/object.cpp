#include "object.h"


CGLObject::CGLObject()
{
	_type = OBJ_UNKNOWN;
	_name = "";
}

CGLObject::~CGLObject()
{
}

CGLObject::obj_type_t CGLObject::get_type()
{
	return _type;
}

void CGLObject::set_name(const std::string &name)
{
	_name = name;
}

std::string CGLObject::get_name()
{
	return _name;
}

