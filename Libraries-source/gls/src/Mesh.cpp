#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstddef>
#include <string>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Vertex1P1N1UV.hpp"
/*
void gls::Mesh::setup_mesh() 
{
	// Create buffers/arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex1P1N1UV), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}*/

void gls::Mesh::setup_mesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex1P1N1UV), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)offsetof(Vertex1P1N1UV, m_n));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)offsetof(Vertex1P1N1UV, m_uv));

	glBindVertexArray(0);
}

void  gls::Mesh::do_shader_plumbing(const gls::Shader & shader, const std::string & v_pos_name, const std::string & v_normal_name, const std::string & v_uv_name) const 
{

	GLint v_pos_loc, v_normal_loc, v_uv_loc;
	
	glBindVertexArray(m_VAO);

	shader.use();
	v_pos_loc = glGetAttribLocation(shader.get_program(), v_pos_name.c_str());
	if (v_pos_loc == -1) {
		std::cerr << "Could not find shader attribute ' " << v_pos_name << "' " << std::endl;
	}

	v_normal_loc = glGetAttribLocation(shader.get_program(), v_normal_name.c_str() );
	if (v_normal_loc == -1) {
		std::cerr << "Could not find shader attribute '" << v_normal_name << "' " << std::endl;
	}

	v_uv_loc = glGetAttribLocation(shader.get_program(), v_uv_name.c_str());
	if (v_uv_loc == -1) {
		std::cerr << "Could not find shader attribute ' " << v_uv_name << " ' " << std::endl;
	}

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(v_pos_loc);
	glVertexAttribPointer(v_pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(v_normal_loc);
	glVertexAttribPointer(v_normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)offsetof(Vertex1P1N1UV, m_n));
	// Vertex Texture Coords
	glEnableVertexAttribArray(v_uv_loc);
	glVertexAttribPointer(v_uv_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV), (GLvoid*)offsetof(Vertex1P1N1UV, m_uv));

	glBindVertexArray(0);

}


void gls::Mesh::render(const gls::Shader & shader) const
{
	GLuint diffuse_number   = 1;
	GLuint specular_number = 1;

	for (GLuint i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string num;
		std::string type = m_textures[i].m_type;

		if (type == "texture_diffuse") {
			num = std::to_string(diffuse_number++);
		}
		else if (type == "texture_specular") {
			num = std::to_string(specular_number++);
		}
		else {
			std::cerr << "Error: Unknow texture type ' " << type << "' " << std::endl;
			continue;
		}
		GLint location = glGetUniformLocation(shader.get_program(), (type + num).c_str());
		if (location == -1) {
			std::cerr << "ERROR: Could not find uniform ' " << type + num << "' in shader program" << std::endl;
		}

		glUniform1f(location, i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	//glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

std::ostream & gls::print_mesh(std::ostream & os, const gls::Mesh & mesh) 
{
	for (GLuint i = 0; i < mesh.m_vertices.size(); ++i) {
		os << "postion attribute = ";
		cgm::print_vec3(os, mesh.m_vertices[i].m_p) << std::endl;
		os << "normal attribute = ";
		cgm::print_vec3(os, mesh.m_vertices[i].m_n) << std::endl;
		os << "texture coordinate attribute = ";
		cgm::print_vec2(os, mesh.m_vertices[i].m_uv) << std::endl;
	}
	for (GLuint i = 0; i < mesh.m_indices.size(); ++i) {
		os << mesh.m_indices[i] << " ";
	}
	os << std::endl;
	os << "Mesh has " << mesh.m_textures.size() << "textures" << std::endl;
	return os;
}