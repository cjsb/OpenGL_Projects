//
//  test.h
//  
//
//  Created by Mateus_Gondim on 02/06/16.
//
//

#ifndef _TEST_HPP

#define _TEST_HPP
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "Shader.hpp"


void initBuffers(GLuint & vao, GLuint & vbo, GLuint & element_buffer, GLuint & text_coord_buff, GLuint & text_id, const Shader & shader );

#endif