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
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, time, node_anim);
		glm::mat4 ScalingM(1.f);
		ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, time, node_anim);
		glm::mat4 RotationM(1.f);
		RotationM = convert_mat(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, time, node_anim);
		glm::mat4 TranslationM(1.f);
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		// Combine the above transformations
		node_trans = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = parent_trans * node_trans;

	// This node is bone, need to calc transform
	if (_scene.bones_map.find(node_name) != _scene.bones_map.end()) {
		uint32_t idx = _scene.bones_map[node_name];
		_scene.bones[idx].final_transform = _scene.transform * GlobalTransformation * _scene.bones[idx].offset_matrix;
	}

	// The children nodes
	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		_read_node_heirarchy(time, node->mChildren[i], GlobalTransformation);
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

void CAssimpParser::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	uint32_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	uint32_t NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void CAssimpParser::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint32_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	uint32_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void CAssimpParser::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint32_t PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	uint32_t NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

uint32_t CAssimpParser::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32_t CAssimpParser::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32_t CAssimpParser::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

