#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"


/**
* @brief:
* Assimp model loading class.
*
*                                           Scene
*                                             |
*                -------------------------------------------------------             
*                |                            |                         |
*             RootNode                     Meshes                    Materials  
*                |                          /\  |                         /\
*       ------------------------             |  |                         |  
*       |                      |             |  \/                        |
*  ChildNode              Mesh_Indexes -------  -------------------------- 
*       |
*    ......
*
*/
class CNGLModel
{
public:
	CNGLModel();
	virtual ~CNGLModel();

	bool load(const string &path);
	void draw(uint32_t shader);

protected:
	void _process_node(aiNode *node, const aiScene *scene);
	ngl_mesh_t _process_mesh(aiMesh *mesh, const aiScene *scene);
	vector<ngl_texture_t> _load_material_textures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int _texture_from_file(const char *path, const string &directory, bool gamma = false);

protected:
	string _dir_path;
	vector<ngl_mesh_t> _meshes;
	vector<ngl_texture_t> _textures_loaded;

	vector<CNGLMesh> _mesh_classes;
};

