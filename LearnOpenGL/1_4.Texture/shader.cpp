#include "shader.h"


CShader::CShader()
{
	_id = 0;
}

CShader::~CShader()
{
}

bool CShader::compile(const char *vs_path_ptr, const char *fs_path_ptr)
{
	ifstream vs_file, fs_file;
	unsigned int vertex_shader = 0, fragment_shader = 0;

	bool success = false;
	do {
		vs_file.open(vs_path_ptr);
		if (vs_file.fail())
			break;
		fs_file.open(fs_path_ptr);
		if (fs_file.fail())
			break;

		stringstream vs_stream, fs_stream;
		string vs_code, fs_code;
		vs_stream << vs_file.rdbuf();
		fs_stream << fs_file.rdbuf();
		vs_code = vs_stream.str();
		fs_code = fs_stream.str();
		const char *vs_code_ptr = vs_code.c_str();
		const char *fs_code_ptr = fs_code.c_str();

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vs_code_ptr, NULL);
		glCompileShader(vertex_shader);
		if (!_check_errors(vertex_shader, SHADER_VERTEX))
			break;
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fs_code_ptr, NULL);
		glCompileShader(fragment_shader);
		if (!_check_errors(fragment_shader, SHADER_FRAGMENT))
			break;
		_id = glCreateProgram();
		glAttachShader(_id, vertex_shader);
		glAttachShader(_id, fragment_shader);
		glLinkProgram(_id);
		if (_check_errors(_id, SHADER_PROGRAM))
			break;

		success = true;
	} while (false);
	vs_file.close();
	fs_file.close();
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return success;
}

void CShader::use()
{
	glUseProgram(_id);
}

void CShader::set_bool(const char *name_ptr, bool value) const
{
	glUniform1i(glGetUniformLocation(_id, name_ptr), (int)value);
}

void CShader::set_int(const char *name_ptr, int value) const
{
	glUniform1i(glGetUniformLocation(_id, name_ptr), value);
}

void CShader::set_float(const char *name_ptr, float value) const
{
	glUniform1f(glGetUniformLocation(_id, name_ptr), value);
}


bool CShader::_check_errors(unsigned int id, shader_type_t type)
{
	int success = 0;
	char info_log[1024];

	switch (type)
	{
	case SHADER_VERTEX:
	case SHADER_FRAGMENT:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 1024, NULL, info_log);
			cout << "Shader Error: " << info_log << endl;
		}
		break;
	case SHADER_PROGRAM:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 1024, NULL, info_log);
			cout << "Shader Error: " << info_log << endl;
		}
		break;
	default:
		break;
	}

	return (0 != success);
}
