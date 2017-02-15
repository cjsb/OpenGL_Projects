#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Vertex1P1N1UV.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vector_operations.hpp"
#include "mat4.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Euler.hpp"

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;
bool    keys[1024];
bool    first_mouse = true;
double curr_x_pos = 0.0f;
double curr_y_pos = 0.0f;
double prev_x_pos = 0.0f;
double prev_y_pos = 0.0f;
double mouse_sensitivity = 0.2;
float  heading, pitch, bank;

float  cam_speed = 6.0f;

/// voxealization data
int voxel_grid_width  = 512;
int voxel_grid_height = 512; 

//// FOR TEST
GLuint g_volTexObj;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void error_callback(int error, const char *description)
{
	std::cerr << description << std::endl;

}

gls::Mesh setup_volume_bbox()
{
	std::vector<gls::Vertex1P1N1UV> vertices;

	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.0f, 0.0f, 0.0f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.0f, 1.0f, 0.0f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.0f, 1.0f, 1.0f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(1.0f, 0.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(1.0f, 0.0f, 1.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(1.0f, 1.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(1.0f, 1.0f, 1.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 1.0f)));

	std::vector<GLuint> indices = {
		1,5,7,
		7,3,1,
		0,2,6,
		6,4,0,
		0,1,3,
		3,2,0,
		7,5,4,
		4,6,7,
		2,3,7,
		7,6,2,
		1,0,4,
		4,5,1
	};

	//gls::Texture diffuse1;
	//gls::load_texture(diffuse1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_diffuse.png", "texture_diffuse");

	//gls::Texture specular1;
	//gls::load_texture(specular1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_specular.png", "texture_specular");

	std::vector<gls::Texture> textures;

	return gls::Mesh(vertices, indices, textures);

}


gls::Mesh plane_mesh()
{
	std::vector<gls::Vertex1P1N1UV> vertices;

	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 0.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 0.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 1.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, -0.5f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, -0.5f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.5f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.5f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 1.0f)));

	std::vector<GLuint> indices = {0, 1, 2, 2, 3 ,0 };

	//gls::Texture diffuse1;
	//gls::load_texture(diffuse1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_diffuse.png", "texture_diffuse");

	//gls::Texture specular1;
	//gls::load_texture(specular1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_specular.png", "texture_specular");

	std::vector<gls::Texture> textures;

	return gls::Mesh(vertices, indices, textures);

}

gls::Mesh triangle_mesh()
{
	std::vector<gls::Vertex1P1N1UV> vertices;

	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 0.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 0.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 1.0f)));
	//vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.0f, 0.5f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 1.0f)));

	std::vector<GLuint> indices = { 0, 1, 2};

	//gls::Texture diffuse1;
	//gls::load_texture(diffuse1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_diffuse.png", "texture_diffuse");

	//gls::Texture specular1;
	//gls::load_texture(specular1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_specular.png", "texture_specular");

	std::vector<gls::Texture> textures;

	return gls::Mesh(vertices, indices, textures);

}


cgm::vec3 do_movement(const cgm::vec3 & pos, const cgm::vec3 & r, const cgm::vec3 & u, const cgm::vec3 & f)
{
	cgm::vec3 new_pos(pos);
	
	if (keys[GLFW_KEY_W]) {
		new_pos.sub_assign(cgm::scale(f, cam_speed * delta_time));
	}	
	if (keys[GLFW_KEY_S]) {
		new_pos.add_assign(cgm::scale(f, cam_speed * delta_time));
	}
	if (keys[GLFW_KEY_A]) {
		new_pos.sub_assign(cgm::scale(r, cam_speed * delta_time));
	}	
	if (keys[GLFW_KEY_D]) {
		new_pos.add_assign(cgm::scale(r, cam_speed * delta_time));
	}
	return new_pos;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (first_mouse) {
		prev_x_pos = xpos;
		prev_y_pos = ypos;
		first_mouse = false;
	}
	float x_offset = prev_x_pos - xpos;
	float y_offset = prev_y_pos - ypos;
	prev_x_pos = xpos;
	prev_y_pos = ypos;

	x_offset *= mouse_sensitivity;
	y_offset *= mouse_sensitivity;

	heading += x_offset;
	pitch   += y_offset;

	if (pitch >= 89.9f) {
		pitch = 90.0f;
	}
	else if (pitch <= -89.9) {
		pitch = -90;
	}
}

GLuint gen_2d_texture(const GLuint text_width, const GLuint text_height)
{
	GLuint back_face_text;
	glGenTextures(1, &back_face_text);
	glBindTexture(GL_TEXTURE_2D, back_face_text);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, text_width, text_height, 0, GL_RGBA, GL_FLOAT, NULL);
	return back_face_text;
}


unsigned int gen_3d_texture(int dim)
{
	/*float* data = new float[dim*dim*dim];
	memset(data, 0, sizeof(float)*dim*dim*dim);

	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_3D, texId);
	//glActiveTexture(GL_TEXTURE0 );
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, dim, dim, dim, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_3D, 0);
	GLenum err = glGetError();
	std::cout << glewGetErrorString(err) << " " << err << std::endl;
	delete[] data;
	return texId;*/

	int size = 4 * dim * dim * dim;

	float* data = (float*)malloc(sizeof(float) * size);
	
	if (data != NULL) {
		for (int i = 0; i < 4 * voxel_grid_width*voxel_grid_width*voxel_grid_width; ++i) {
			data[i] = 0.0f;
		}
	}

	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_3D, tex_id);
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexStorage3D(GL_TEXTURE_3D, 10, GL_RGBA8, voxel_grid_width, voxel_grid_width, voxel_grid_width);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, voxel_grid_width, voxel_grid_width, voxel_grid_width, 0, GL_RGBA, GL_FLOAT, &data[0]);
	
	glGenerateMipmap(GL_TEXTURE_3D);

	glBindTexture(GL_TEXTURE_3D, 0);
	glCheckError();
	//GLenum err = glGetError();
	//std::cout << glewGetErrorString(err) << " " << err << std::endl;
	free(data);
	return tex_id;
}

GLuint gen_framebuffer(const GLuint text_id, const GLuint text_width, const GLuint tex_height) 
{
	GLuint depth_buffer;
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, text_width, tex_height);

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text_id, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Could not generate framebuffer object" << std::endl;
	}
		

	return framebuffer;
}

void fface_bface_renderer(GLFWwindow * window, GLuint & bf_texture, GLuint & ff_texture, const gls::Shader & backface_shader, const gls::Shader & frontface_shader  ,cgs::Camera & camera ) 
{
	
	glViewport(0, 0, voxel_grid_width, voxel_grid_height);

	// Start 01 Step, render the back faces of the volume boulding box to a framebuffer //

	

	// generate the framebuffer and assign the 2d texture to it
	GLuint bf_framebuffer = gen_framebuffer(bf_texture, voxel_grid_width, voxel_grid_height);

	gls::Mesh volume = setup_volume_bbox();

	//gls::Shader backface_shader("../../Applications-source/model_loader/shaders/backface.vert", "../../Applications-source/model_loader/shaders/backface.frag");
	backface_shader.use();

	//create the 2d texture
	//cgs::Camera camera;
	//camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	//camera.get_transform().set_object_to_upright(cgm::mat4());
	//camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 3.0f));

	cgm::mat4 model = cgm::translate(cgm::vec3(-0.5f, -0.5f, -0.5f));
	model.concat_assign(cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), -35.0f));
	//model.concat_assign(cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), 25.0f));
	
	GLint model_loc = backface_shader.get_uniform_location("model");
	GLint  view_loc = backface_shader.get_uniform_location("view");
	GLint  proj_loc = backface_shader.get_uniform_location("projection");

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	//here you bind the framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bf_framebuffer);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//while (!glfwWindowShouldClose(window)) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	backface_shader.use();
	volume.render(backface_shader);

	glfwSwapBuffers(window);
	glfwPollEvents();
	//}
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///--------------------------------END 01--------------------------------------------- //

	//  STEP 02 -  Render the front faces to texture////////////////////////////////////////////////


	GLuint ff_framebuffer = gen_framebuffer(ff_texture, voxel_grid_width, voxel_grid_height);

	//gls::Shader ff_shader("../../Applications-source/model_loader/shaders/frontface.vert", "../../Applications-source/model_loader/shaders/frontface.frag");
	frontface_shader.use();

	model_loc = frontface_shader.get_uniform_location("model");
	view_loc = frontface_shader.get_uniform_location("view");
	proj_loc = frontface_shader.get_uniform_location("projection");

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ff_framebuffer);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//while (!glfwWindowShouldClose(window)) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	frontface_shader.use();
	volume.render(frontface_shader);

	glfwSwapBuffers(window);
	glfwPollEvents();
	//}
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// delete the framebuffers
	glDeleteFramebuffers(1, &bf_framebuffer);
	glDeleteFramebuffers(1, &ff_framebuffer);

}

void render_voxels_test(const GLuint & tex_3d, GLFWwindow * window)
{
	cgs::Camera camera;
	camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	camera.get_transform().set_object_to_upright(cgm::mat4());
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 3.0f));
	GLuint bf_texture, ff_texture;
	
	bf_texture = gen_2d_texture(voxel_grid_width, voxel_grid_height);
	ff_texture = gen_2d_texture(voxel_grid_width, voxel_grid_height);

	gls::Shader backface_shader("../../Applications-source/model_loader/shaders/backface.vert", "../../Applications-source/model_loader/shaders/backface.frag");
	gls::Shader frontface_shader("../../Applications-source/model_loader/shaders/frontface.vert", "../../Applications-source/model_loader/shaders/frontface.frag");

	// maybe pass a model matrix for the cube position
	fface_bface_renderer(window, bf_texture, ff_texture, backface_shader, frontface_shader, camera);

	//------------------- Step 03 - Ray marching----------------------------------------------------//

	////////////////////////////////////DRAW A PLANE IN FRONT OF THE CAMERA/////////////////////////////////////////////////////////////
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gls::Shader ray_marching_sh("../../Applications-source/model_loader/shaders/ray_marching.vert", "../../Applications-source/model_loader/shaders/ray_marching.frag");
	ray_marching_sh.use();

	gls::Mesh near_clipping_plane = plane_mesh();
	
	//orient the plane to match the camera's near clipping plane
	float left, r, l, b, t, n, f;
	camera.get_bnd(l, r, b, t, n, f);


	cgm::mat4 model = cgm::scale(r * 2.0f, t * 2.0f, 1.0f);
	model.concat_assign(cgm::translate(cgm::vec3(0.0f, 0.0f, -n - 0.0001f)));
	model.concat_assign(camera.get_transform().object_to_upright());
	model.concat_assign(cgm::translate(camera.get_transform().get_position()));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GLuint model_loc = ray_marching_sh.get_uniform_location("model");
	GLuint view_loc = ray_marching_sh.get_uniform_location("view");
	GLuint proj_loc = ray_marching_sh.get_uniform_location("projection");

	GLint backface_loc = ray_marching_sh.get_uniform_location("exit_points");
	GLint frontface_loc = ray_marching_sh.get_uniform_location("entry_points");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bf_texture);
	glUniform1i(backface_loc, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ff_texture);
	glUniform1i(frontface_loc, 2);

	GLint screen_size_loc = ray_marching_sh.get_uniform_location("screen_size");
	cgm::vec2 screen_size(voxel_grid_width, voxel_grid_height);

	GLint voxel_grid_loc = ray_marching_sh.get_uniform_location("voxel_grid_width");


	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	glUniform2fv(screen_size_loc, 1, screen_size.value_ptr());
	glUniform1i(voxel_grid_loc, voxel_grid_width);


	glBindTexture(GL_TEXTURE_3D, tex_3d);
	glBindImageTexture(0, tex_3d, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);

	cgm::mat4 view;
	while (!glfwWindowShouldClose(window)) {
		
		GLfloat current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		cgs::Euler euler(heading, pitch, bank);
		cgm::mat4 cam_orientation = euler.get_rotation_mat4();
		camera.get_transform().set_object_to_upright(cam_orientation);

		cgm::vec3 right = cam_orientation.p();
		cgm::vec3 up = cam_orientation.q();
		cgm::vec3 forward = cam_orientation.r();

		cgm::vec3 pos = do_movement(camera.get_transform().get_position(), right, up, forward);

		camera.get_transform().set_position(pos);
		view = cgm::invert_orthogonal(camera.get_transform().object_to_world());

	

		fface_bface_renderer(window, bf_texture, ff_texture, backface_shader, frontface_shader, camera);

		ray_marching_sh.use();

		//camera.get_bnd(l, r, b, t, n, f);


		model = cgm::scale(r * 2.0f, t * 2.0f, 1.0f);
		model.concat_assign(cgm::translate(cgm::vec3(0.0f, 0.0f, -n - 0.0001f)));
		model.concat_assign(camera.get_transform().object_to_upright());
		model.concat_assign(cgm::translate(camera.get_transform().get_position()));

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bf_texture);
		glUniform1i(backface_loc, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, ff_texture);
		glUniform1i(frontface_loc, 2);

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

		//glBindTexture(GL_TEXTURE_3D, tex_3d);
		//glBindImageTexture(0, tex_3d, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ray_marching_sh.use();
		near_clipping_plane.render(ray_marching_sh);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
}

void render_voxels( const GLuint & tex_3d , GLFWwindow * window) 
{
	
	glViewport(0, 0, voxel_grid_width, voxel_grid_height);

	// Start 01 Step, render the back faces of the volume boulding box to a framebuffer //
	
	//generate the framebuffer's texture
	GLuint bf_texture   =  gen_2d_texture(voxel_grid_width, voxel_grid_height);
	glCheckError();

	// generate the framebuffer and assign the 2d texture to it
	GLuint bf_framebuffer  =  gen_framebuffer(bf_texture, voxel_grid_width, voxel_grid_height);
	glCheckError();
	gls::Mesh volume =  setup_volume_bbox();
	
	gls::Shader backface_shader("../../Applications-source/model_loader/shaders/backface.vert", "../../Applications-source/model_loader/shaders/backface.frag");
	backface_shader.use();
	glCheckError();
	//create the 2d texture
	cgs::Camera camera;
	camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	camera.get_transform().set_object_to_upright(cgm::mat4());
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 3.0f));

	cgm::mat4 model = cgm::translate(cgm::vec3(-0.5f, -0.5f, -0.5f));
	model.concat_assign(cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), -35.0f));
	model.concat_assign(cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), 25.0f));
	GLint model_loc = backface_shader.get_uniform_location("model");
	GLint  view_loc = backface_shader.get_uniform_location("view");
	GLint  proj_loc = backface_shader.get_uniform_location("projection");

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	//here you bind the framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bf_framebuffer);
	glCheckError();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	//while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		backface_shader.use();
		volume.render(backface_shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	//}
	glCheckError();
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &bf_framebuffer);
	glCheckError();
	glDeleteProgram(backface_shader.get_program());
	glCheckError();
	///--------------------------------END 01--------------------------------------------- //
	
	//  STEP 02 -  Render the front faces to texture////////////////////////////////////////////////

	GLuint ff_texture = gen_2d_texture(voxel_grid_width, voxel_grid_height);
	glCheckError();
	
	GLuint ff_framebuffer = gen_framebuffer(ff_texture, voxel_grid_width, voxel_grid_height);
	glCheckError();
	
	gls::Shader ff_shader("../../Applications-source/model_loader/shaders/frontface.vert", "../../Applications-source/model_loader/shaders/frontface.frag");
	ff_shader.use();
	glCheckError();

	model_loc  =  ff_shader.get_uniform_location("model");
	view_loc   =  ff_shader.get_uniform_location("view");
	proj_loc   =  ff_shader.get_uniform_location("projection");

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ff_framebuffer);
	glCheckError();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//while (!glfwWindowShouldClose(window)) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ff_shader.use();
	volume.render(ff_shader);

	glfwSwapBuffers(window);
	glfwPollEvents();
	//}
	glCheckError();
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1, &ff_framebuffer);
	glCheckError();
	glDeleteProgram(ff_shader.get_program());
	glCheckError();
	///////////////////////////////////////////////////////////////////////////////////////////////////

	//------------------- Step 03 - Ray marching----------------------------------------------------//

	gls::Shader ray_marching_sh("../../Applications-source/model_loader/shaders/ray_marching.vert", "../../Applications-source/model_loader/shaders/ray_marching.frag");
	ray_marching_sh.use();
	glCheckError();

	////////////////////////////////////DRAW A PLANE IN FRONT OF THE CAMERA/////////////////////////////////////////////////////////////
	gls::Mesh near_clipping_plane = plane_mesh();

	//orient the plane to match the camera's near clipping plane
	float left, right, bottom, top, near, far;
	camera.get_bnd(left, right, bottom, top, near, far);


	model = cgm::scale(right * 2.0f, top * 2.0f, 1.0f);
	model.concat_assign(cgm::translate(cgm::vec3(0.0f, 0.0f, -near - 0.0001f)));
	model.concat_assign(camera.get_transform().object_to_upright());
	model.concat_assign(cgm::translate(camera.get_transform().get_position()));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	model_loc = ray_marching_sh.get_uniform_location("model");
	view_loc = ray_marching_sh.get_uniform_location("view");
	proj_loc = ray_marching_sh.get_uniform_location("projection");

	GLint backface_loc   =  ray_marching_sh.get_uniform_location("exit_points");
	GLint frontface_loc   = ray_marching_sh.get_uniform_location("entry_points");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bf_texture);
	glUniform1i(backface_loc, 1);
	glCheckError();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ff_texture);
	glUniform1i(frontface_loc, 2);
	glCheckError();
	/*
	GLint volume_loc = ray_marching_sh.get_uniform_location("voxel_image");
	glActiveTexture(GL_TEXTURE3);
	 glBindTexture(GL_TEXTURE_3D, tex_3d);
	glUniform1i(volume_loc, 3);
	*/
	GLint screen_size_loc = ray_marching_sh.get_uniform_location("screen_size");
	cgm::vec2 screen_size(voxel_grid_width, voxel_grid_height);

	GLint voxel_grid_loc = ray_marching_sh.get_uniform_location("voxel_grid_width");


	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	glUniform2fv(screen_size_loc, 1, screen_size.value_ptr());
	glUniform1i(voxel_grid_loc, voxel_grid_width);
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	
	glBindTexture(GL_TEXTURE_3D, tex_3d);
	glBindImageTexture(0, tex_3d, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
	glCheckError();
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE1);
		ray_marching_sh.use();
		near_clipping_plane.render(ray_marching_sh);
		//glCheckError();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glCheckError();
	//---------------------------- END 03 ------------------------------------------------- ///
}

void voxels_slice_visualization(const GLuint & tex_3d, GLFWwindow * window)
{
	gls::Shader visualization_shader("../../Applications-source/model_loader/shaders/slice_visualization.vert", "../../Applications-source/model_loader/shaders/slice_visualization.frag");
	visualization_shader.use();
	gls::Mesh mesh = plane_mesh();

	cgs::Camera camera;
	camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	camera.get_transform().set_object_to_upright(cgm::mat4());
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, -3.0f));

	//orient the plane to match the camera's near clipping plane
	float left, right, bottom, top, near, far;
	camera.get_bnd(left, right, bottom, top, near, far);

	cgm::mat4 model;
	model = cgm::scale(right * 2.0f, top * 2.0f, 1.0f);
	model.concat_assign(cgm::translate(cgm::vec3(0.0f, 0.0f, -near - 0.0001f)));
	model.concat_assign(camera.get_transform().object_to_upright());
	model.concat_assign(cgm::translate(camera.get_transform().get_position()));


	//cgs::Transform obj_transf;
	//obj_transf.set_position(cgm::vec3(0.0f, 0.0f, 0.0f));

	GLint  model_loc = visualization_shader.get_uniform_location("model");
	GLint  view_loc = visualization_shader.get_uniform_location("view");
	GLint  proj_loc = visualization_shader.get_uniform_location("projection");
	GLint  slice_loc = visualization_shader.get_uniform_location("slice");
	GLint  axis_loc  = visualization_shader.get_uniform_location("axis");

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (cgm::invert_orthogonal(camera.get_transform().object_to_world())).value_ptr());
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());

	int slice_val = 0;
	glUniform1i(slice_loc, slice_val); // z axis
	int axis = 1;
	glUniform1i(axis_loc, axis);

	glBindTexture(GL_TEXTURE_3D, tex_3d);
	glBindImageTexture(0, tex_3d, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		visualization_shader.use();
		mesh.render(visualization_shader);

		if (keys[GLFW_KEY_W]) {
			++slice_val;
			std::cout << " z axis slice is " << slice_val << std::endl;
		}
		if (keys[GLFW_KEY_S]) {
			--slice_val;
			std::cout << "z axis slice is " << slice_val << std::endl;
		}
		if ((keys[GLFW_KEY_X])) {
			axis = 1;
			std::cout << "axis slice is X" << std::endl;
		}
		if ((keys[GLFW_KEY_Y])) {
			axis = 2;
			std::cout << "axis slice is Y" << std::endl;
		}
		if ((keys[GLFW_KEY_Z])) {
			axis = 3;
			std::cout << "axis slice is Z " << std::endl;
		}
		glUniform1i(slice_loc, slice_val);
		glUniform1i(axis_loc, axis);
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
}

void voxelize_scene(const gls::Model & m, const gls::Shader & vs, const GLuint & tex_id , GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, voxel_grid_width, voxel_grid_height);
	
	//Disable some fixed-function opeartions
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	cgm::mat4 ortho = cgm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f - 1.0f, 3.0f);
	
	cgs::Transform obj_transf;
	obj_transf.set_object_to_upright(cgm::concat_mat4(cgm::scale(0.12f, 0.12f, 0.12f), cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f),0.0f )) );
	obj_transf.set_position(cgm::vec3(0.0f, 0.0f, 0.0f));
	

	cgs::Camera camera;
	camera.set_mode(cgs::Camera::ORTHOGRAPHIC);
	camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	
	
	/// ORTHOGONAL PROJECTION ALONG THE X AXIS ///////////////////
	camera.get_transform().set_object_to_upright(cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), 90.0f));
	camera.get_transform().set_position(cgm::vec3(2.0f, 0.0f, 0.0f));
	
	cgm::mat4 mvp_x = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_x.concat_assign(ortho);
	GLint mvp_x_loc =  vs.get_uniform_location("u_mvp_x");
	glUniformMatrix4fv(mvp_x_loc, 1, GL_FALSE, mvp_x.value_ptr());

	/// -------------------------------------/////////////////////
	
	/// ORTHOGONAL PROJECTION ALONG THE Y AXIS //////
	camera.get_transform().set_object_to_upright(cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), -90.0f));
	camera.get_transform().set_position(cgm::vec3(0.0f, 2.0f, 0.0f));

	cgm::mat4 mvp_y = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_y.concat_assign(ortho);
	GLint mvp_y_loc = vs.get_uniform_location("u_mvp_y");
	glUniformMatrix4fv(mvp_y_loc, 1, GL_FALSE, mvp_y.value_ptr());
	//-----------------------------------------/////
	
	// ORTHOGONAL PROJECTION ALONG THE Z AXIS //

	camera.get_transform().set_object_to_upright(cgm::mat4());
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 2.0f));

	cgm::mat4 mvp_z = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_z.concat_assign(ortho);
	GLint mvp_z_loc = vs.get_uniform_location("u_mvp_z");
	glUniformMatrix4fv(mvp_z_loc, 1, GL_FALSE, mvp_z.value_ptr());
	// --------------------------------------//
	
	glUniform1i(vs.get_uniform_location("u_voxel_grid_width"), voxel_grid_width);
	glUniform1i(vs.get_uniform_location("u_voxel_grid_height"), voxel_grid_height);

	
	glBindTexture(GL_TEXTURE_3D, tex_id);
	glBindImageTexture(0, tex_id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	//while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vs.use();
		m.render(vs);

		glfwSwapBuffers(window);
		glfwPollEvents();
	//}

	/*glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	static  float data[4 * 128 * 128 * 128] = { 0.0f };
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, data);
	int r, c, s;
	r = c = s = 0;
	int count = 0;
	for (auto & i : data) {
		++count;
		if (i != 0) {
			std::cout << i << " ";
			std::cout << "count= " << count << " ";
		}
		
	}
	std::cout << std::endl;*/

	glBindTexture(GL_TEXTURE_3D, 0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//gls::Shader volume_shader("../../Applications-source/model_loader/shaders/ray_marching.vert", "../../Applications-source/model_loader/shaders/ray_marching.frag");
	glDeleteProgram(vs.get_program());
	glCheckError();
	render_voxels_test( tex_id, window);
	//voxels_slice_visualization(tex_id, window);
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

	GLFWwindow *window = glfwCreateWindow(voxel_grid_width, voxel_grid_height, "Model Loader", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Initialize GLEW 
	glewExperimental = GL_TRUE;
	glewInit();


	//gls::Mesh mesh = setup_box();
	

	//gls::Shader shader("../../Applications-source/model_loader/shaders/vertex.vert", "../../Applications-source/model_loader/shaders/fragment2.frag");
	gls::Shader shader("C:/Users/mateu/Documents/Projects/Applications/model_loader/source/shaders/vertex2.vert", "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/shaders/fragment2.frag");
	
	
	gls::Shader voxelize_shader("../../Applications-source/model_loader/shaders/voxelize.vert", "../../Applications-source/model_loader/shaders/voxelize.frag", "../../Applications-source/model_loader/shaders/voxelize.geom");
	
	//shader.use();
	voxelize_shader.use();
	gls::Model model_suit("../../Resources/Cow/cow.obj");
	

	GLuint tex_3d_id = gen_3d_texture(voxel_grid_width);

	voxelize_scene(model_suit ,voxelize_shader, tex_3d_id, window);
	std::cout << "finish" << std::endl;
	
	//g_volTexObj = initVol3DTex("../../Resources/head256.raw", 256, 256, 225);
	//render_voxels(g_volTexObj, window);
	return 0;



	//model_suit.do_shader_plumbing(shader, "v_position", "v_normal", "v_uv");
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	cgm::vec3 light_color(1.0f, 1.0f, 1.0f);
	
	GLint light_color_loc = shader.get_uniform_location( "light_color");
	
	cgm::vec3 light_pos(0.0f, 8.0f, 3.0f);
	GLint light_pos_loc = shader.get_uniform_location("light_pos");


	GLint cam_pos_loc = shader.get_uniform_location("cam_pos");
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//set matrixes
	cgs::Transform model_transform;
	model_transform.set_position(cgm::vec3(0.0f, 0.0f, 0.0f));
	cgs::Camera camera;
	camera.set_far_clipping_plane(160.0f);
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 2.0f));
	cgm::mat4  aux = cgm::scale(0.2f, 0.2f, 0.2f);
	cgm::mat4   model = cgm::concat_mat4(aux, model_transform.object_to_world());
	cgm::mat4   view = cgm::invert_orthogonal(camera.get_transform().object_to_world());
	
	
	GLint model_loc = shader.get_uniform_location("model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	

	GLint view_loc = shader.get_uniform_location("view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());

	GLint p_loc = shader.get_uniform_location("projection");
	/////// MATRIXES FOR VOXEALIZATION //////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////
	glUniform3fv(light_color_loc, 1, &light_color.x);
	glUniform3fv(light_pos_loc, 1,  &light_pos.x);
	glUniform3fv(cam_pos_loc, 1, &(camera.get_transform().get_position().x));

	int curr_width, curr_height;
	glEnable(GL_DEPTH_TEST); 
	float theta = 5.0f;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		GLfloat current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		cgs::Euler euler(heading, pitch, bank);
		cgm::mat4 cam_orientation = euler.get_rotation_mat4();
		camera.get_transform().set_object_to_upright(cam_orientation);
		
		cgm::vec3 right    =  cam_orientation.p();
		cgm::vec3 up       =  cam_orientation.q();
		cgm::vec3 forward  =  cam_orientation.r();

		cgm::vec3 pos = do_movement(camera.get_transform().get_position() ,right, up, forward);

		camera.get_transform().set_position(pos);
		view = cgm::invert_orthogonal(camera.get_transform().object_to_world());

		float device_aspect_ratio;

		glfwGetFramebufferSize(window, &curr_width, &curr_height);
		glViewport(0, 0, curr_width, curr_height);

		if ((camera.get_image_width() != curr_width) || (camera.get_image_height() != curr_height)) {
			camera.scale_film_gate(curr_width, curr_height);
			glUniformMatrix4fv(p_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
		}

		glUniformMatrix4fv(p_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		model_suit.render(shader);
		//mesh.render(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	   glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}