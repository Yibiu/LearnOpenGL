#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"


class CNGLModel
{
public:
	CNGLModel();
	virtual ~CNGLModel();

	bool load(const string &path, bool gamma = false);
	void draw(unsigned int shader);

protected:
	void _process_node(aiNode *node, const aiScene *scene);
	CNGLMesh _process_mesh(aiMesh *mesh, const aiScene *scene);
	vector<ngl_texture_t> _load_material_textures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int _texture_from_file(const char *path, const string &directory, bool gamma = false);

protected:
	vector<ngl_texture_t> textures_loaded;
	vector<CNGLMesh> meshes;
	string directory;
	bool gammaCorrection;
};

