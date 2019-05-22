#pragma once
#include <map>
#include "shader.h"
#include "texture.h"


/**
* @brief:
* Resources class for OpenGL
*/
class CGLFactory
{
public:
	CGLFactory();
	virtual ~CGLFactory();

	CGLShader* create_shader(const std::string &name);
	void destroy_shader(const std::string &name);
	CGLTexture* create_texture(const std::string &name);
	void destroy_texture(const std::string &name);

protected:
	std::map<std::string, CGLShader *> _shaders;
	std::map<std::string, CGLTexture *> _textures;
};
