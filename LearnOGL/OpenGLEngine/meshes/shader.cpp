#include "shader.h"


CGLShader::CGLShader()
{
	_type = CGLObject::OBJ_SHADER;
	_name = "";

	_shader = 0;
}

CGLShader::~CGLShader()
{
}

bool CGLShader::init(const std::string &vs_str, const std::string &fs_str, bool file)
{
	bool success = false;

	do {
		std::string vs_code;
		std::string fs_code;
		if (file) {
			try {
				std::ifstream vs_file(vs_str);
				std::ifstream fs_file(fs_str);
				std::stringstream vs_stream, fs_stream;
				vs_stream << vs_file.rdbuf();
				fs_stream << fs_file.rdbuf();
				vs_file.close();
				fs_file.close();

				vs_code = vs_stream.str();
				fs_code = fs_stream.str();
			}
			catch (std::exception e) {
				break;
			}
		}
		else {
			vs_code = vs_str;
			fs_code = fs_str;
		}

		if (!_compile_source(vs_code.c_str(), fs_code.c_str()))
			break;

		success = true;
	} while (false);

	return success;
}

void CGLShader::uninit()
{
	if (_shader > 0) {
		glDeleteProgram(_shader);
		_shader = 0;
	}
}

bool CGLShader::use()
{
	if (_shader > 0) {
		glUseProgram(_shader);
		return true;
	}

	return false;
}

uint64_t CGLShader::get()
{
	return _shader;
}

bool CGLShader::set_int(const std::string &name, int value)
{
	if (_shader > 0) {
		glUniform1i(glGetUniformLocation(_shader, name.c_str()), value);
		return true;
	}

	return false;
}

bool CGLShader::set_float(const std::string &name, float value)
{
	if (_shader > 0) {
		glUniform1f(glGetUniformLocation(_shader, name.c_str()), value);
		return true;
	}

	return false;
}

bool CGLShader::set_vec2f(const std::string &name, const glm::vec2 &value)
{
	if (_shader > 0) {
		glUniform2f(glGetUniformLocation(_shader, name.c_str()), value.x, value.y);
		return true;
	}

	return false;
}

bool CGLShader::set_vec3f(const std::string &name, const glm::vec3 &value)
{
	if (_shader > 0) {
		glUniform3f(glGetUniformLocation(_shader, name.c_str()), value.x, value.y, value.z);
		return true;
	}

	return false;
}

bool CGLShader::set_vec4f(const std::string &name, const glm::vec4 &value)
{
	if (_shader > 0) {
		glUniform4f(glGetUniformLocation(_shader, name.c_str()), value.x, value.y, value.z, value.w);
		return true;
	}

	return false;
}

bool CGLShader::set_mat4(const std::string &name, const glm::mat4 &value)
{
	if (_shader > 0) {
		glUniformMatrix4fv(glGetUniformLocation(_shader, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		return true;
	}

	return false;
}


bool CGLShader::_compile_source(const GLchar *vs_ptr, const GLchar *fs_ptr)
{
	bool success = false;

	do {
		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, &vs_ptr, NULL);
		glCompileShader(vshader);
		if (!_check_compile_err(vshader, SHADER_VERTEX))
			break;

		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader, 1, &fs_ptr, NULL);
		glCompileShader(fshader);
		if (!_check_compile_err(fshader, SHADER_FRAGMENT))
			break;

		_shader = glCreateProgram();
		glAttachShader(_shader, vshader);
		glAttachShader(_shader, fshader);
		glLinkProgram(_shader);
		if (!_check_compile_err(_shader, SHADER_PROGRAM))
			break;

		glDeleteShader(vshader);
		glDeleteShader(fshader);
		success = true;
	} while (false);

	return success;
}

bool CGLShader::_check_compile_err(GLuint obj, shader_type_t type)
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
