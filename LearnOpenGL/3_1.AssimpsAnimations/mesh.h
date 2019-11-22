#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
using namespace std;


typedef struct _ngl_vertice
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
} ngl_vertice_t;

typedef struct _ngl_texture
{
	uint32_t id;
	string type;
	string path;
} ngl_texture_t;

typedef struct _ngl_mesh
{
	vector<ngl_vertice_t> vertices;
	vector<uint32_t> indices;
	vector<ngl_texture_t> textures;
} ngl_mesh_t;


/**
* @brief:
* Mesh class for model building.
*/
class CNGLMesh
{
public:
	CNGLMesh();
	virtual ~CNGLMesh();

	void load(const vector<ngl_vertice_t> &vertices, const vector<uint32_t> &indices,
		const vector<ngl_texture_t> &textures);
	void draw(uint32_t shader);

protected:
	void _setup_mesh();

protected:
	vector<ngl_vertice_t> _vertices;
	vector<uint32_t> _indices;
	vector<ngl_texture_t> _textures;
	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;
};


