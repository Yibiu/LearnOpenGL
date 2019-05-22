#pragma once
#include "object.h"


/**
* @brief:
* Texture class for OpenGL
*/
class CGLTexture : public CGLObject
{
public:
	CGLTexture();
	virtual ~CGLTexture();

	bool init(const std::string &file, bool empty, bool flip = false);
	void uninit();
	bool update(uint32_t width, uint32_t height, color_fmt_t fmt, const uint8_t *data_ptr);
	bool use();
	uint64_t get();

protected:
	GLuint _texture;
};
