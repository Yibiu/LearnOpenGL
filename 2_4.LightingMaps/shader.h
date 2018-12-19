#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad\glad.h"
using namespace std;


/**
* @brief:
*	Shader class for shader loading.
*/
class CShader
{
public:
	typedef enum _shader_type
	{
		SHADER_VERTEX = 0,
		SHADER_FRAGMENT,
		SHADER_PROGRAM
	} shader_type_t;

public:
	CShader();
	virtual ~CShader();

	unsigned int get_id();
	bool compile(const char *vs_path_ptr, const char *fs_path_ptr);
	void use();
	void set_bool(const char *name_ptr, bool value) const;
	void set_int(const char *name_ptr, int value) const;
	void set_float(const char *name_ptr, float value) const;

protected:
	bool _check_errors(unsigned int id, shader_type_t type);

protected:
	unsigned int _id;
};

