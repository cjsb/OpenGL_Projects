//opengl, glew, glfw
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include "Model.hpp"
#include "Mesh.hpp"
#include "Vertex1P1N1UV.hpp"
#include "Texture.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void gls::Model::load_model(const std::string & path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR:ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_path = path.substr(0, path.find_last_of('/'));

	// ***DEBUG*****
	std::cout << "loading model : " << path << std::endl << "In directory:" << m_path << std::endl;

	process_node(scene->mRootNode, scene);
}

void gls::Model::process_node(aiNode * node, const aiScene * scene) 
{
	//process the node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene));
	}
	//process each node's child
	for (GLuint i = 0; i < node->mNumChildren; ++i) {
		process_node(node -> mChildren[i], scene);
	}
}

gls::Mesh gls::Model::process_mesh(aiMesh * mesh, const aiScene * scene) 
{
	std::vector<Vertex1P1N1UV>  vertices;
	std::vector<GLuint>         indices;
	std::vector<Texture>        textures;

	//process the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
		Vertex1P1N1UV vertex;
		//position
		vertex.m_p = cgm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		
		//normal
		vertex.m_n = cgm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		//texture coordinates
		if (mesh->mTextureCoords[0]) {
			vertex.m_uv = cgm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.m_uv = cgm::vec2();
		}
		vertices.push_back(vertex);
	}
	
	//process the mesh's indices
	for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//process the mesh's materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<gls::Texture> gls::Model::load_material_textures(aiMaterial * material, aiTextureType type, const std::string & type_name) 
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		Texture texture;
		gls::load_texture(texture, m_path + '/' + str.C_Str() , type_name);
		textures.push_back(texture);
	}
	return textures;
}

void gls::Model::do_shader_plumbing(const gls::Shader & shader, const std::string & v_pos_name, const std::string & v_normal_name, const std::string & v_uv_name) const 
{
	for (GLuint i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].do_shader_plumbing(shader, v_pos_name, v_normal_name, v_uv_name);
	}
}

void gls::Model::render(const gls::Shader & shader) const 
{
	for (GLuint i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].render(shader);
	}
}

std::ostream & gls::print_model(std::ostream & os, const gls::Model & model)
{
	for (GLuint i = 0; i < model.m_meshes.size(); ++i) {
		gls::print_mesh(os, model.m_meshes[i]) << std::endl;
	}

	return os;
}