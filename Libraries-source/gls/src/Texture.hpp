#ifndef _TEXTURE_HPP

#define _TEXTURE_HPP
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
//TODO: Create a "destroy texture" function

namespace gls {
	struct Texture {
		GLuint	      m_id;
		std::string   m_type;
		bool          m_is_img_loaded = false;
	};

	void load_texture(Texture & texture, const std::string & path, const std::string & type);
}

#endif // !_TEXTURE_HPP

