#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
using namespace std;


typedef struct _ngl_vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
} ngl_vertex_t;

typedef struct _ngl_texture
{
	unsigned int id;
	string type;
	string path;
} ngl_texture_t;


class CNGLMesh
{
public:
	CNGLMesh();
	virtual ~CNGLMesh();

	void load(const vector<ngl_vertex_t> &vertices, const vector<unsigned int> &indices, 
		const vector<ngl_texture_t> &textures);
	void draw(unsigned int shader);

protected:
	void _setup_mesh();

protected:
	vector<ngl_vertex_t> _vertices;
	vector<unsigned int> _indices;
	vector<ngl_texture_t> _textures;
	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;
};


