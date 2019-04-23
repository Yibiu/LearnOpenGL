#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#include "object.h"


/**
* @brief:
* Shader of OpenGL object.
*/
class CNGLShader : public CNGLObject
{
public:
	typedef enum _shader_type
	{
		SHADER_UNKNOWN = 0,
		SHADER_VERTEX,
		SHADER_FRAGMENT,
		SHADER_PROGRAM
	} shader_type_t;

public:
	CNGLShader();
	virtual ~CNGLShader();

	bool init(const GLchar *vpath_ptr, const GLchar *fpath_ptr);
	void unint();
	void use();
	GLuint get_id();

	void set_int(const GLchar *name, GLint value);
	void set_float(const GLchar *name, GLfloat value);
	void set_vec2f(const GLchar *name, const glm::vec2 &value);
	void set_vec3f(const GLchar *name, const glm::vec3 &value);
	void set_vec4f(const GLchar *name, const glm::vec4 &value);
	void set_mat4(const GLchar *name, const glm::mat4 &value);

protected:
	bool _compile_source(const GLchar *vsource_ptr, const GLchar *fsource_ptr);
	bool _check_compile_err(GLuint obj, shader_type_t type);

protected:
	GLuint _id;
};


