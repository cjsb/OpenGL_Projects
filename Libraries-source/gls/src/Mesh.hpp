#ifndef _MESH_HPP

#define _MESH_HPP
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex1P1N1UV.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace gls {
	class Mesh {
	friend std::ostream & print_mesh(std::ostream & os, const Mesh & mesh);
	public:
		Mesh(const std::vector<Vertex1P1N1UV> & vertices, const std::vector<GLuint> & indices, const std::vector<Texture> & textures) 
		{
			m_vertices = vertices;
			m_indices  = indices;
			m_textures = textures;
			setup_mesh();
		}
	void  do_shader_plumbing(const Shader & shader,const std::string & v_pos_name, const std::string & v_normal_name, const std::string & v_uv_name) const;
	void  render(const Shader & shader) const;
	const std::vector<Vertex1P1N1UV> & get_vertices() const { return m_vertices; }
	private:
		void setup_mesh();
		std::vector<Vertex1P1N1UV>  m_vertices;
		std::vector<GLuint>			m_indices;
		std::vector<Texture>		m_textures;
		GLuint						m_VAO, m_VBO, m_EBO;
	};

	std::ostream & print_mesh(std::ostream & os, const Mesh & mesh);
}

#endif // !_MESH_HPP
