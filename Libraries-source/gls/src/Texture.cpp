#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include "Texture.hpp"
#include "SOIL.h"

void gls::load_texture(gls::Texture &texture, const std::string & path, const std::string & type) 
{
	// ******DEBUG *****
	std::cout << "Loading texture from file: " << path << std::endl;

	if (texture.m_is_img_loaded) {
		glDeleteTextures(1, &texture.m_id);
	}
	else {
		texture.m_is_img_loaded = true;
	}
	glGenTextures(1, &texture.m_id);
	glBindTexture(GL_TEXTURE_2D, texture.m_id);

	texture.m_type = type;
	if (type == "texture_diffuse") {
		std::cout << "Created a " << type << std::endl;
	}
	else if (type == "texture_specular") {
		std::cout << "Created a " << type << std::endl;
	}
	else {
		std::cerr << "Error created a texture of a unkown type ' " << type << "' " << std::endl;
	}
	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}