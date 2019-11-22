#pragma once
#include <vector>
#include <map>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
// glm
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)


//////////////////////////////////////
typedef struct _ap_weight
{
	uint32_t bone_idx;
	float bone_weight;
} ap_weight_t;

typedef struct _ap_vertice
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	std::vector<ap_weight_t> weights; // Bone in mesh bones
	std::vector<ap_weight_t> weights1; // Bone in total bones
} ap_vertice_t;

typedef struct _ap_bone
{
	std::string name;
	glm::mat4 offset_matrix;
	glm::mat4 final_transform;
} ap_bone_t;

typedef struct _ap_mesh
{
	uint32_t material_idx;
	std::vector<ap_vertice_t> vertices;
	std::vector<uint32_t> indices;
	std::vector<ap_bone_t> bones; // Mesh bones

	// Not used
	uint32_t base_vertices;
	uint32_t base_indices;
} ap_mesh_t;

typedef struct _ap_texture
{
	std::string path;
} ap_texture_t;

typedef struct _sp_material
{
	std::vector<ap_texture_t> diffuses;
	std::vector<ap_texture_t> speculars;
	std::vector<ap_texture_t> ambient;
} ap_material_t;

typedef struct _ap_node
{
	std::string name; // This name is same with bone name
	std::vector<uint32_t> mesh_idxes;
} ap_node_t;

typedef struct _ap_scene
{
	glm::mat4 transform;
	std::vector<ap_mesh_t> meshes; // All meshes
	std::vector<ap_material_t> materials; // All materials
	std::map<std::string, uint32_t> bones_map; // All bones extract from meshes
	std::vector<ap_bone_t> bones; // All bones extract from meshes

	std::vector<ap_node_t> nodes; // All nodes
} ap_scene_t;


/**
* @brief:
* CAssimpParser
*
* NOTE:
* 1. Bones in one mesh are mesh-related bones, so they may be the same bones in another mesh;
* 2. One of animation in Animation[] is a serial of movies(in ticks). The channels record the related nodes;
* 3. Bones are defined in nodes(same name).
*/
class CAssimpParser
{
public:
	CAssimpParser();
	virtual ~CAssimpParser();

	ap_scene_t& get_scene();
	bool load(const std::string &file);
	void transform(float time_sec, std::vector<glm::mat4> &trans); // Calc current transform

protected:
	bool _parse_scene(const aiScene *scene_ptr);
	void _parse_mesh(const aiMesh *mesh_ptr);
	void _parse_material(const aiMaterial *material_ptr);
	void _parse_node(const aiNode *node_ptr);

	void _read_node_heirarchy(float time, const aiNode *node, const glm::mat4 &parent_trans);
	const aiNodeAnim* _find_node_in_animation(const std::string &node, const aiAnimation *animation);
	void _calc_interpolated_scaling(aiVector3D &out, float time, const aiNodeAnim *node_anim);
	void _calc_interpolated_rotation(aiQuaternion &out, float time, const aiNodeAnim *node_anim);
	void _calc_interpolated_position(aiVector3D &out, float time, const aiNodeAnim *node_anim);
	uint32_t _find_scaling(float time, const aiNodeAnim *node_anim);
	uint32_t _find_rotation(float time, const aiNodeAnim *node_anim);
	uint32_t _find_position(float time, const aiNodeAnim *node_anim);

protected:
	std::string _dir_path;

	Assimp::Importer _importer;
	const aiScene *_ai_scene;
	ap_scene_t _scene;
};



