#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


CGLTexture::CGLTexture()
{
	_type = OBJ_TEXTURE;
	_name = "";

	_texture = 0;
}

CGLTexture::~CGLTexture()
{
}

bool CGLTexture::init(const std::string &file, bool empty, bool flip)
{
	bool error = false;

	if (empty) {
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		int width, height, channels;
		uint8_t *data_ptr = NULL;
		do {
			stbi_set_flip_vertically_on_load(flip);
			data_ptr = stbi_load(file.c_str(), &width, &height, &channels, 0);
			if (NULL == data_ptr) {
				error = true;
				break;
			}

			uint32_t fmt = GL_RGB;
			if (4 == channels) {
				fmt = GL_RGBA;
			}
			else if (3 == channels) {
				fmt = GL_RGB;
			}
			else if (1 == channels) {
				fmt = GL_RED;
			}
			else {
				error = true;
				break;
			}
			glGenTextures(1, &_texture);
			glBindTexture(GL_TEXTURE_2D, _texture);
			glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, data_ptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		} while (false);
		if (NULL != data_ptr) {
			stbi_image_free(data_ptr);
		}
	}

	return (!error);
}

void CGLTexture::uninit()
{
	if (_texture > 0) {
		glDeleteTextures(1, &_texture);
		_texture = 0;
	}
}

bool CGLTexture::update(uint32_t width, uint32_t height, color_fmt_t fmt, const uint8_t *data_ptr)
{
	if (_texture > 0) {
		uint32_t fmt = GL_RGB;
		switch (fmt)
		{
		case COLOR_FMT_GRAY:
			fmt = GL_RED;
			break;
		case COLOR_FMT_RGB:
			fmt = GL_RGB;
			break;
		case COLOR_FMT_BGR:
			fmt = GL_BGR;
			break;
		case COLOR_FMT_RGBA:
			fmt = GL_RGBA;
			break;
		case COLOR_FMT_BGRA:
			fmt = GL_BGRA;
			break;
		default:
			return false;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, data_ptr);
		return true;
	}

	return false;
}

bool CGLTexture::use()
{
	if (_texture > 0) {
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

	return false;
}

uint64_t CGLTexture::get()
{
	return _texture;
}

