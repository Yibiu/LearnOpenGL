#include "stb_image.h"
#include "texture.h"


CNGLTexture::CNGLTexture()
{
	_type = CNGLObject::OBJ_TEXTURE;
	_name = "";

	_id = 0;
	_width = 0;
	_height = 0;
	_fmt = 0;
}

CNGLTexture::~CNGLTexture()
{
}

bool CNGLTexture::init(GLchar *path_ptr)
{
	int width, height, channels;
	//stbi_set_flip_vertically_on_load(true);
	uint8_t *data_ptr = stbi_load(path_ptr, &width, &height, &channels, 0);
	if (NULL == data_ptr)
		return false;

	_width = width;
	_height = height;
	_fmt = 0;
	switch (channels)
	{
	case 1:
		_fmt = GL_RED;
		break;
	case 3:
		_fmt = GL_RGB;
		break;
	case 4:
		_fmt = GL_RGBA;
		break;
	default:
		break;
	}
	if (0 == _fmt)
		return false;
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, _fmt, _width, _height, 0, _fmt, GL_UNSIGNED_BYTE, data_ptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data_ptr);

	return true;
}

void CNGLTexture::uninit()
{
	glDeleteTextures(1, &_id);
}

void CNGLTexture::update(const GLchar *data_ptr)
{
	glTexImage2D(GL_TEXTURE_2D, 0, _fmt, _width, _height, 0, _fmt, GL_UNSIGNED_BYTE, data_ptr);
}

void CNGLTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}



