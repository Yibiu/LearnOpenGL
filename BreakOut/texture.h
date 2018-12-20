#pragma once
#include <stdint.h>
#include <string>
#include "glad/glad.h"
#include "object.h"


/**
* @brief
* Texture of OpenGL object.
*/
class CNGLTexture : public CNGLObject
{
public:
	CNGLTexture();
	virtual ~CNGLTexture();

	bool init(GLchar *path_ptr);
	void uninit();
	void update(const uint8_t *data_ptr);
	void bind();

protected:
	GLuint _id;
	GLuint _width;
	GLuint _height;
	GLuint _fmt;
};
