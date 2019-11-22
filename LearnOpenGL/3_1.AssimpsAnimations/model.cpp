#include "stb_image.h"
#include "model.h"


CNGLModel::CNGLModel()
{
	_dir_path = "";
	_meshes.clear();
	_textures_loaded.clear();
}

CNGLModel::~CNGLModel()
{
}

bool CNGLModel::load(const string &path)
{
	bool success = false;
	do {
		size_t index = path.find_last_of('/');
		if (string::npos == index)
			break;
		_dir_path = path.substr(0, index);

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
			break;

		// Parse to deep...
		_meshes.clear();
		_textures_loaded.clear();
		_process_node(scene->mRootNode, scene);

		// Data to class
		_mesh_classes.clear();
		for (uint32_t i = 0; i < _meshes.size(); i++) {
			CNGLMesh mesh_class;
			mesh_class.load(_meshes[i].vertices, _meshes[i].indices, _meshes[i].textures);
			_mesh_classes.push_back(mesh_class);
		}

		success = true;
	} while (false);

	return success;
}

void CNGLModel::draw(uint32_t shader)
{
	for (uint32_t i = 0; i < _mesh_classes.size(); i++)
		_mesh_classes[i].draw(shader);
}


void CNGLModel::_process_node(aiNode *node, const aiScene *scene)
{
	// Process meshes of current node
	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_process_mesh(mesh, scene));
	}
	// Process child of current node
	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		_process_node(node->mChildren[i], scene);
	}
}

ngl_mesh_t CNGLModel::_process_mesh(aiMesh *mesh, const aiScene *scene)
{
	ngl_mesh_t mesh_data;

	// Vertices of current mesh
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		ngl_vertice_t vertice;

		glm::vec2 tmp_vec2;
		glm::vec3 tmp_vec3;
		// Position
		tmp_vec3.x = mesh->mVertices[i].x;
		tmp_vec3.y = mesh->mVertices[i].y;
		tmp_vec3.z = mesh->mVertices[i].z;
		vertice.position = tmp_vec3;
		// Normal
		tmp_vec3.x = mesh->mNormals[i].x;
		tmp_vec3.y = mesh->mNormals[i].y;
		tmp_vec3.z = mesh->mNormals[i].z;
		vertice.normal = tmp_vec3;
		// Texcoords
		if (mesh->mTextureCoords[0]) {
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			tmp_vec2.x = mesh->mTextureCoords[0][i].x;
			tmp_vec2.y = mesh->mTextureCoords[0][i].y;
			vertice.texcoords = tmp_vec2;
		}
		else {
			vertice.texcoords = glm::vec2(0.0f, 0.0f);
		}
		// Tangent
		tmp_vec3.x = mesh->mTangents[i].x;
		tmp_vec3.y = mesh->mTangents[i].y;
		tmp_vec3.z = mesh->mTangents[i].z;
		vertice.tangent = tmp_vec3;
		// Bitangent
		tmp_vec3.x = mesh->mBitangents[i].x;
		tmp_vec3.y = mesh->mBitangents[i].y;
		tmp_vec3.z = mesh->mBitangents[i].z;
		vertice.bitangent = tmp_vec3;

		mesh_data.vertices.push_back(vertice);
	}

	// Indices of current mesh
	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			mesh_data.indices.push_back(face.mIndices[j]);
	}

	// Textures of current mesh
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	vector<ngl_texture_t> diffuse_textures = _load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	mesh_data.textures.insert(mesh_data.textures.end(), diffuse_textures.begin(), diffuse_textures.end());
	vector<ngl_texture_t> specular_textures = _load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
	mesh_data.textures.insert(mesh_data.textures.end(), specular_textures.begin(), specular_textures.end());
	std::vector<ngl_texture_t> normal_textures = _load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
	mesh_data.textures.insert(mesh_data.textures.end(), normal_textures.begin(), normal_textures.end());
	std::vector<ngl_texture_t> height_textures = _load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
	mesh_data.textures.insert(mesh_data.textures.end(), height_textures.begin(), height_textures.end());

	return mesh_data;
}

vector<ngl_texture_t> CNGLModel::_load_material_textures(aiMaterial *mat, aiTextureType type, string type_name)
{
	vector<ngl_texture_t> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check texture repeat(Optimization)
		bool skip = false;
		for (uint32_t j = 0; j < _textures_loaded.size(); j++) {
			if (std::strcmp(_textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(_textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			ngl_texture_t texture;
			texture.id = _texture_from_file(str.C_Str(), _dir_path);
			texture.type = type_name;
			texture.path = str.C_Str();
			textures.push_back(texture);
			_textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int CNGLModel::_texture_from_file(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		stbi_image_free(data);
	}

	return textureID;
}


