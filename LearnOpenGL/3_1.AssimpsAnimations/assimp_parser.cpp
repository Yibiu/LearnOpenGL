#include "assimp_parser.h"


static inline glm::mat4 convert_mat(const aiMatrix4x4 &amat)
{
	glm::mat4 mat;
	mat[0][0] = amat.a1; mat[0][1] = amat.a2; mat[0][2] = amat.a3; mat[0][3] = amat.a4;
	mat[1][0] = amat.b1; mat[1][1] = amat.b2; mat[1][2] = amat.b3; mat[1][3] = amat.b4;
	mat[2][0] = amat.c1; mat[2][1] = amat.c2; mat[2][2] = amat.c3; mat[2][3] = amat.c4;
	mat[3][0] = amat.d1; mat[3][1] = amat.d2; mat[3][2] = amat.d3; mat[3][3] = amat.d4;
	return glm::transpose(mat);
}

static inline glm::mat4 convert_mat(const aiMatrix3x3 &amat)
{
	glm::mat4 mat;
	mat[0][0] = amat.a1; mat[0][1] = amat.a2; mat[0][2] = amat.a3; mat[0][3] = 0.f;
	mat[1][0] = amat.b1; mat[1][1] = amat.b2; mat[1][2] = amat.b3; mat[1][3] = 0.f;
	mat[2][0] = amat.c1; mat[2][1] = amat.c2; mat[2][2] = amat.c3; mat[2][3] = 0.f;
	mat[3][0] = 0.f    ; mat[3][1] = 0.f    ; mat[3][2] = 0.f    ; mat[3][3] = 1.f;
	return glm::transpose(mat);
}

// CSkeletalAnimation
CAssimpParser::CAssimpParser()
{
	_dir_path = "";
	_scene.meshes.clear();
	_scene.materials.clear();
	_scene.bones_map.clear();
	_scene.bones.clear();
	_scene.nodes.clear();
}

CAssimpParser::~CAssimpParser()
{
}

ap_scene_t& CAssimpParser::get_scene()
{
	return _scene;
}

bool CAssimpParser::load(const std::string &file)
{
	std::size_t index = file.find_last_of("/");
	if (index == std::string::npos) {
		_dir_path = ".";
	}
	else if (0 == index) {
		_dir_path = "/";
	}
	else {
		_dir_path = file.substr(0, index);
	}

	_ai_scene = _importer.ReadFile(file.c_str(), ASSIMP_LOAD_FLAGS);
	if (NULL == _ai_scene)
		return false;

	// Global inverse matrix
	aiMatrix4x4 trans = _ai_scene->mRootNode->mTransformation;
	trans.Inverse();
	_scene.transform = convert_mat(trans);

	// Parse scene
	return _parse_scene(_ai_scene);
}

void CAssimpParser::transform(float time_sec, std::vector<glm::mat4> &trans)
{
	float ticks_per_second = (float)(_ai_scene->mAnimations[0]->mTicksPerSecond != 0 ? _ai_scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float time_in_ticks = time_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, (float)_ai_scene->mAnimations[0]->mDuration); // 当前对应的动画时间(in ticks)

	// Read nodes to update bones transform
	glm::mat4 identity(1.f);
	_read_node_heirarchy(animation_time, _ai_scene->mRootNode, identity);

	// Return the bones transform
	trans.clear();
	for (uint32_t i = 0; i < _scene.bones.size(); i++) {
		trans.push_back(_scene.bones[i].final_transform);
	}
}

bool CAssimpParser::_parse_scene(const aiScene *scene_ptr)
{
	_scene.meshes.clear();
	_scene.materials.clear();
	_scene.bones_map.clear();
	_scene.bones.clear();
	_scene.nodes.clear();

	// Parse all meshes
	uint32_t num_meshes = scene_ptr->mNumMeshes;
	for (uint32_t i = 0; i < num_meshes; i++) {
		const aiMesh *mesh_ptr = scene_ptr->mMeshes[i];
		_parse_mesh(mesh_ptr);
	}

	// Parse all materials
	uint32_t num_materials = scene_ptr->mNumMaterials;
	for (uint32_t i = 0; i < num_materials; i++) {
		const aiMaterial *material_ptr = scene_ptr->mMaterials[i];
		_parse_material(material_ptr);
	}

	// Parse all nodes
	_parse_node(_ai_scene->mRootNode);

	return true;
}

void CAssimpParser::_parse_mesh(const aiMesh *mesh_ptr)
{
	ap_mesh_t mesh;

	// Parse all vertices of this mesh
	for (uint32_t i = 0; i < mesh_ptr->mNumVertices; i++) {
		const aiVector3D *pos = &(mesh_ptr->mVertices[i]);
		const aiVector3D *nor = &(mesh_ptr->mNormals[i]);
		const aiVector3D *tex = mesh_ptr->HasTextureCoords(0) ? &(mesh_ptr->mTextureCoords[0][i]) : &aiVector3D(0.f, 0.f, 0.f);

		ap_vertice_t vertice;
		vertice.position = glm::vec3(pos->x, pos->y, pos->z);
		vertice.normal = glm::vec3(nor->x, nor->y, nor->z);
		vertice.texcoord = glm::vec2(tex->x, tex->y);
		mesh.vertices.push_back(vertice);
	}

	// Parse all related bones of this mesh
	for (uint32_t i = 0; i < mesh_ptr->mNumBones; i++) {
		// Mesh bones
		ap_bone_t bone;
		bone.name = mesh_ptr->mBones[i]->mName.data;
		bone.offset_matrix = convert_mat(mesh_ptr->mBones[i]->mOffsetMatrix);
		mesh.bones.push_back(bone);

		// Total bones
		uint32_t bone_idx = 0;
		if (_scene.bones_map.find(bone.name) == _scene.bones_map.end()) {
			bone_idx = _scene.bones.size();
			_scene.bones.push_back(bone);
			_scene.bones_map[bone.name] = bone_idx;
		}
		else {
			bone_idx = _scene.bones_map[bone.name];
		}

		// Update the effects of this bone
		for (uint32_t j = 0; j < mesh_ptr->mBones[i]->mNumWeights; j++) {
			uint32_t id = mesh_ptr->mBones[i]->mWeights[j].mVertexId;
			float wt = mesh_ptr->mBones[i]->mWeights[j].mWeight;

			// Mesh bones
			ap_weight_t weight;
			weight.bone_idx = i;
			weight.bone_weight = wt;
			mesh.vertices[id].weights.push_back(weight);

			// Total bones
			weight.bone_idx = bone_idx;
			mesh.vertices[id].weights1.push_back(weight);
		}
	}

	// Parse all indices of this mesh(only support triangles)
	for (uint32_t i = 0; i < mesh_ptr->mNumFaces; i++) {
		const aiFace &face = mesh_ptr->mFaces[i];
		if (3 == face.mNumIndices) {
			mesh.indices.push_back(face.mIndices[0]);
			mesh.indices.push_back(face.mIndices[1]);
			mesh.indices.push_back(face.mIndices[2]);
		}
	}

	mesh.material_idx = mesh_ptr->mMaterialIndex;
	_scene.meshes.push_back(mesh);
}

void CAssimpParser::_parse_material(const aiMaterial *material_ptr)
{
	ap_material_t material;

	// Parse all diffuse materials
	for (uint32_t i = 0; i < material_ptr->GetTextureCount(aiTextureType_DIFFUSE); i++) {
		aiString str;
		if (AI_SUCCESS == material_ptr->GetTexture(aiTextureType_DIFFUSE, i, &str)) {
			ap_texture_t texture;
			texture.path = _dir_path + "/" + str.C_Str();
			material.diffuses.push_back(texture);
		}
	}

	// Parse all specular materials
	for (uint32_t i = 0; i < material_ptr->GetTextureCount(aiTextureType_SPECULAR); i++) {
		aiString str;
		if (AI_SUCCESS == material_ptr->GetTexture(aiTextureType_SPECULAR, i, &str)) {
			ap_texture_t texture;
			texture.path = _dir_path + "/" + str.C_Str();
			material.speculars.push_back(texture);
		}
	}

	// Parse all ambient materials
	for (uint32_t i = 0; i < material_ptr->GetTextureCount(aiTextureType_AMBIENT); i++) {
		aiString str;
		if (AI_SUCCESS == material_ptr->GetTexture(aiTextureType_AMBIENT, i, &str)) {
			ap_texture_t texture;
			texture.path = _dir_path + "/" + str.C_Str();
			material.ambient.push_back(texture);
		}
	}

	_scene.materials.push_back(material);
}

void CAssimpParser::_parse_node(const aiNode *node_ptr)
{
	ap_node_t node;

	// Parse all meshes of this node
	for (uint32_t i = 0; i < node_ptr->mNumMeshes; i++) {
		node.mesh_idxes.push_back(node_ptr->mMeshes[i]);
	}
	_scene.nodes.push_back(node);

	// Parse all children of this node
	for (uint32_t i = 0; i < node_ptr->mNumChildren; i++) {
		_parse_node(node_ptr->mChildren[i]);
	}
}

void CAssimpParser::_read_node_heirarchy(float time, const aiNode *node, const glm::mat4 &parent_trans)
{
	const aiAnimation *anim = _ai_scene->mAnimations[0];

	std::string node_name(node->mName.data);
	glm::mat4 node_trans = convert_mat(node->mTransformation);
	const aiNodeAnim* node_anim = _find_node_in_animation(node_name, anim);
	if (node_anim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D scaling;
		_calc_interpolated_scaling(scaling, time, node_anim);
		glm::mat4 scaling_m(1.f);
		scaling_m = glm::scale(scaling_m, glm::vec3(scaling.x, scaling.y, scaling.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion rotation;
		_calc_interpolated_rotation(rotation, time, node_anim);
		glm::mat4 rotation_m(1.f);
		rotation_m = convert_mat(rotation.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D translation;
		_calc_interpolated_position(translation, time, node_anim);
		glm::mat4 translation_m(1.f);
		translation_m = glm::translate(translation_m, glm::vec3(translation.x, translation.y, translation.z));

		// Combine the above transformations
		node_trans = translation_m * rotation_m * scaling_m;
	}

	glm::mat4 global_transformation = parent_trans * node_trans;

	// This node is bone, need to calc transform
	if (_scene.bones_map.find(node_name) != _scene.bones_map.end()) {
		uint32_t idx = _scene.bones_map[node_name];
		_scene.bones[idx].final_transform = _scene.transform * global_transformation * _scene.bones[idx].offset_matrix;
	}

	// The children nodes
	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		_read_node_heirarchy(time, node->mChildren[i], global_transformation);
	}
}

const aiNodeAnim* CAssimpParser::_find_node_in_animation(const std::string &node, const aiAnimation *animation)
{
	for (uint32_t i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim *node_anim = animation->mChannels[i];

		if (0 == node.compare(node_anim->mNodeName.data)) {
			return node_anim;
		}
	}

	return NULL;
}

void CAssimpParser::_calc_interpolated_scaling(aiVector3D &out, float time, const aiNodeAnim *node_anim)
{
	// Need at least two values to interpolate...
	if (node_anim->mNumScalingKeys == 1) {
		out = node_anim->mScalingKeys[0].mValue;
		return;
	}

	// FIXME: What if the index is the max index?
	uint32_t scaling_index = _find_scaling(time, node_anim);
	uint32_t next_scaling_index = (scaling_index + 1);
	assert(next_scaling_index < node_anim->mNumScalingKeys);
	float delta_time = (float)(node_anim->mScalingKeys[next_scaling_index].mTime - node_anim->mScalingKeys[scaling_index].mTime);
	float factor = (time - (float)node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiVector3D &start = node_anim->mScalingKeys[scaling_index].mValue;
	const aiVector3D &end = node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;
	out = start + factor * delta;
}

void CAssimpParser::_calc_interpolated_rotation(aiQuaternion &out, float time, const aiNodeAnim *node_anim)
{
	// Need at least two values to interpolate...
	if (node_anim->mNumRotationKeys == 1) {
		out = node_anim->mRotationKeys[0].mValue;
		return;
	}

	// FIXME: What if the index is the max index?
	uint32_t rotation_index = _find_rotation(time, node_anim);
	uint32_t next_rotation_index = (rotation_index + 1);
	assert(next_rotation_index < node_anim->mNumRotationKeys);
	float delta_time = (float)(node_anim->mRotationKeys[next_rotation_index].mTime - node_anim->mRotationKeys[rotation_index].mTime);
	float factor = (time - (float)node_anim->mRotationKeys[rotation_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiQuaternion &start_rotationQ = node_anim->mRotationKeys[rotation_index].mValue;
	const aiQuaternion &end_rotationQ = node_anim->mRotationKeys[next_rotation_index].mValue;
	aiQuaternion::Interpolate(out, start_rotationQ, end_rotationQ, factor);
	out = out.Normalize();
}

void CAssimpParser::_calc_interpolated_position(aiVector3D &out, float time, const aiNodeAnim *node_anim)
{
	// Need at least two values to interpolate...
	if (node_anim->mNumPositionKeys == 1) {
		out = node_anim->mPositionKeys[0].mValue;
		return;
	}

	// FIXME: What if the index is the max index?
	uint32_t position_index = _find_position(time, node_anim);
	uint32_t next_position_index = (position_index + 1);
	assert(next_position_index < node_anim->mNumPositionKeys);
	float delta_time = (float)(node_anim->mPositionKeys[next_position_index].mTime - node_anim->mPositionKeys[position_index].mTime);
	float factor = (time - (float)node_anim->mPositionKeys[position_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiVector3D &start = node_anim->mPositionKeys[position_index].mValue;
	const aiVector3D &end = node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;
	out = start + factor * delta;
}

uint32_t CAssimpParser::_find_scaling(float time, const aiNodeAnim *node_anim)
{
	assert(node_anim->mNumScalingKeys > 0);

	for (uint32_t i = 0; i < node_anim->mNumScalingKeys - 1; i++) {
		if (time < (float)node_anim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32_t CAssimpParser::_find_rotation(float time, const aiNodeAnim *node_anim)
{
	assert(node_anim->mNumRotationKeys > 0);

	for (uint32_t i = 0; i < node_anim->mNumRotationKeys - 1; i++) {
		if (time < (float)node_anim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32_t CAssimpParser::_find_position(float time, const aiNodeAnim *node_anim)
{
	for (uint32_t i = 0; i < node_anim->mNumPositionKeys - 1; i++) {
		if (time < (float)node_anim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

