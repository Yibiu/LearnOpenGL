#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "object.h"


/**
* @brief:
* Shader class for OpenGL
*/
class CGLShader : public CGLObject
{
public:
	typedef enum _shader_type
	{
		SHADER_UNKNOWN = 0x00,
		SHADER_VERTEX,
		SHADER_FRAGMENT,
		SHADER_PROGRAM
	} shader_type_t;

public:
	CGLShader();
	virtual ~CGLShader();

	bool init(const std::string &vs_str, const std::string &fs_str, bool file);
	void uninit();
	bool use();
	uint64_t get();

	bool set_int(const std::string &name, int value);
	bool set_float(const std::string &name, float value);
	bool set_vec2f(const std::string &name, const glm::vec2 &value);
	bool set_vec3f(const std::string &name, const glm::vec3 &value);
	bool set_vec4f(const std::string &name, const glm::vec4 &value);
	bool set_mat4(const std::string &name, const glm::mat4 &value);

protected:
	bool _compile_source(const GLchar *vs_ptr, const GLchar *fs_ptr);
	bool _check_compile_err(GLuint obj, shader_type_t type);

protected:
	GLuint _shader;
};

