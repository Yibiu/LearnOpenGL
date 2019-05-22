#include "factory.h"


CGLFactory::CGLFactory()
{
	_shaders.clear();
	_textures.clear();
}

CGLFactory::~CGLFactory()
{
}

CGLShader* CGLFactory::create_shader(const std::string &name)
{
	CGLShader *shader_ptr = NULL;

	std::map<std::string, CGLShader *>::iterator iter = _shaders.find(name);
	if (iter != _shaders.end()) {
		shader_ptr = iter->second;
	}
	else {
		shader_ptr = new CGLShader;
		if (NULL != shader_ptr) {
			shader_ptr->set_name(name);
		}
	}

	return shader_ptr;
}

void CGLFactory::destroy_shader(const std::string &name)
{
	std::map<std::string, CGLShader *>::iterator iter = _shaders.find(name);
	if (iter != _shaders.end()) {
		CGLShader *shader_ptr = iter->second;
		shader_ptr->uninit();
		delete shader_ptr;

		_shaders.erase(iter);
	}
}

CGLTexture* CGLFactory::create_texture(const std::string &name)
{
	CGLTexture *texture_ptr = NULL;

	std::map<std::string, CGLTexture *>::iterator iter = _textures.find(name);
	if (iter != _textures.end()) {
		texture_ptr = iter->second;
	}
	else {
		texture_ptr = new CGLTexture;
		if (NULL != texture_ptr) {
			texture_ptr->set_name(name);
		}
	}

	return texture_ptr;
}

void CGLFactory::destroy_texture(const std::string &name)
{
	std::map<std::string, CGLTexture *>::iterator iter = _textures.find(name);
	if (iter != _textures.end()) {
		CGLTexture *texture_ptr = iter->second;
		texture_ptr->uninit();
		delete texture_ptr;

		_textures.erase(iter);
	}
}

