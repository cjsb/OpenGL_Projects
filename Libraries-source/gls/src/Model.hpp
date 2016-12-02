#ifndef _MODEL_HPP

#define _MODEL_HPP
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Shader.hpp"

namespace gls {
	class Model {
	friend std::ostream & print_model(std::ostream & os, const gls::Model & model);
	public:
		Model(const std::string & path)  
		{
			load_model(path);
		}
		void do_shader_plumbing(const gls::Shader & shader, const std::string & v_pos_name, const std::string & v_normal_name, const std::string & v_uv_name) const;
		void render(const Shader & shader) const;
	private:
		void			      load_model(const std::string & path);
		void			      process_node(aiNode * node, const aiScene * scene);
		Mesh			      process_mesh(aiMesh * mesh, const aiScene * scene);
		std::vector<Texture>  load_material_textures(aiMaterial * material, aiTextureType type, const std::string & type_name);
		
		std::vector<Mesh>     m_meshes;
		std::string           m_path;

	};
	std::ostream & print_model(std::ostream & os, const gls::Model & model);
}

#endif // !_MODEL_HPP
