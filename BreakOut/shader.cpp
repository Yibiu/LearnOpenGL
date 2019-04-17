#include "shader.h"


CNGLShader::CNGLShader()
{
	_type = CNGLObject::OBJ_SHADER;
	_name = "";

	_id = 0;
}

CNGLShader::~CNGLShader()
{
}

bool CNGLShader::init(const GLchar *vpath_ptr, const GLchar *fpath_ptr)
{
	bool success = false;

	do {
		std::string vertex_code;
		std::string fragment_code;
		try {
			std::ifstream vshader_file(vpath_ptr);
			std::ifstream fshader_file(fpath_ptr);
			std::stringstream vshader_stream, fshader_stream;
			vshader_stream << vshader_file.rdbuf();
			fshader_stream << fshader_file.rdbuf();
			vshader_file.close();
			fshader_file.close();

			vertex_code = vshader_stream.str();
			fragment_code = fshader_stream.str();
		}
		catch (std::exception e) {
			break;
		}

		const GLchar *vshader_source = vertex_code.c_str();
		const GLchar *fshader_source = fragment_code.c_str();
		if (!_compile_source(vshader_source, fshader_source))
			break;
		
		success = true;
	} while (false);

	return success;
}

void CNGLShader::unint()
{
	glDeleteProgram(_id);
}

void CNGLShader::use()
{
	glUseProgram(_id);
}

GLuint CNGLShader::get_id()
{
	return _id;
}

void CNGLShader::set_int(const GLchar *name, GLint value)
{
	glUniform1i(glGetUniformLocation(_id, name), value);
}

void CNGLShader::set_float(const GLchar *name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(_id, name), value);
}

void CNGLShader::set_vec2f(const GLchar *name, const glm::vec2 &value)
{
	glUniform2f(glGetUniformLocation(_id, name), value.x, value.y);
}

void CNGLShader::set_vec3f(const GLchar *name, const glm::vec3 &value)
{
	glUniform3f(glGetUniformLocation(_id, name), value.x, value.y, value.z);
}

void CNGLShader::set_vec4f(const GLchar *name, const glm::vec4 &value)
{
	glUniform4f(glGetUniformLocation(_id, name), value.x, value.y, value.z, value.w);
}

void CNGLShader::set_mat4(const GLchar *name, const glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
}


bool CNGLShader::_compile_source(const GLchar *vsource_ptr, const GLchar *fsource_ptr)
{
	bool success = false;

	do {
		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, &vsource_ptr, NULL);
		glCompileShader(vshader);
		if (!_check_compile_err(vshader, SHADER_VERTEX))
			break;

		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader, 1, &fsource_ptr, NULL);
		glCompileShader(fshader);
		if (!_check_compile_err(fshader, SHADER_FRAGMENT))
			break;

		_id = glCreateProgram();
		glAttachShader(_id, vshader);
		glAttachShader(_id, fshader);
		glLinkProgram(_id);
		if (!_check_compile_err(_id, SHADER_PROGRAM))
			break;

		glDeleteShader(vshader);
		glDeleteShader(fshader);
		success = true;
	} while (false);

	return success;
}

bool CNGLShader::_check_compile_err(GLuint obj, shader_type_t type)
{
	bool error = false;

	GLint success;
	GLchar infoLog[1024];
	switch (type)
	{
	case SHADER_VERTEX:
	case SHADER_FRAGMENT:
		glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
		if (!success) {
			error = true;
			glGetShaderInfoLog(obj, 1024, NULL, infoLog);
		}
		break;
	case SHADER_PROGRAM:
		glGetProgramiv(obj, GL_LINK_STATUS, &success);
		if (!success) {
			error = true;
			glGetProgramInfoLog(obj, 1024, NULL, infoLog);
		}
		break;
	default:
		break;
	}

	return (!error);
}

