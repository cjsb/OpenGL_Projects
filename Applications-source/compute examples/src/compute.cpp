#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "compute.hpp"
#include "Shader.hpp"

#include <iostream>


void error_callback(int error, const char *description)
{
	std::cerr << description << std::endl;

}

void example(GLFWwindow *window)
{
	GLuint output_image;
	GLuint  render_vao;
	GLuint  render_vbo;
	
	gls::Shader comp("../../Applications-source/compute examples/shaders/comp_ex01.comp");

	// This is the texture that the compute program will write into
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);

	gls::Shader render("../../Applications-source/compute examples/shaders/render_image.vert", "../../Applications-source/compute examples/shaders/render_image.frag");

	// This is the VAO containing the data to draw the quad (including its associated VBO)
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &render_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_vbo);
	static const float verts[] =
	{
		-1.0f, -1.0f, 0.5f, 1.0f,
		1.0f, -1.0f, 0.5f, 1.0f,
		1.0f,  1.0f, 0.5f, 1.0f,
		-1.0f,  1.0f, 0.5f, 1.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// compute 

	// Activate the compute program and bind the output texture image
	comp.use();
	glBindImageTexture(0, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(8, 16, 1);

	// Now bind the texture for rendering _from_
	glBindTexture(GL_TEXTURE_2D, output_image);

	// Clear, select the rendering program and draw a full screen quad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	while (!glfwWindowShouldClose(window)) {
		render.use();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char *argv[])
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(256, 256, "compute", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Initialize GLEW 
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	example(window);

	std::cout << "OK" << std::endl;
	return 0;

}