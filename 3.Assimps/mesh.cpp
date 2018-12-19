#include "mesh.h"


CNGLMesh::CNGLMesh()
{
	_vertices.clear();
	_indices.clear();
	_textures.clear();
	_VAO = 0;
	_VBO = 0;
	_EBO = 0;
}

CNGLMesh::~CNGLMesh()
{
}

void CNGLMesh::load(const vector<ngl_vertex_t> &vertices, const vector<unsigned int> &indices,
	const vector<ngl_texture_t> &textures)
{
	_vertices = vertices;
	_indices = indices;
	_textures = textures;

	_setup_mesh();
}

void CNGLMesh::draw(unsigned int shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = _textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


void CNGLMesh::_setup_mesh()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(ngl_vertex_t), &_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ngl_vertex_t), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ngl_vertex_t), (void*)offsetof(ngl_vertex_t, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ngl_vertex_t), (void*)offsetof(ngl_vertex_t, texcoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ngl_vertex_t), (void*)offsetof(ngl_vertex_t, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ngl_vertex_t), (void*)offsetof(ngl_vertex_t, bitangent));
	glBindVertexArray(0);
}

