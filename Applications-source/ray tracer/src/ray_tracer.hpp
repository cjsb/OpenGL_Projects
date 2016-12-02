//
//  ray_tracer.hpp
//  
//
//  Created by Mateus Gondim on 9/9/16.
//
//

#ifndef _RAY_TRACER_HPP

#define _RAY_TRACER_HPP
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

void set_up(GLuint & vao, GLuint & vbo, GLuint & ebo, const Shader & shader);


#endif