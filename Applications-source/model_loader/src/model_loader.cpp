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



gls::Mesh setup_box()
{
	std::vector<gls::Vertex1P1N1UV> vertices;

	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, -0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(1.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.5f, -0.5f), cgm::vec3(0.0f, 0.0f, -1.0f), cgm::vec2(0.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, -0.5f, 0.5f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, -0.5f, 0.5f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 0.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(0.5f, 0.5f, 0.5f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(1.0f, 1.0f)));
	vertices.push_back(gls::Vertex1P1N1UV(cgm::vec3(-0.5f, 0.5f, 0.5f), cgm::vec3(0.0f, 0.0f, 1.0f), cgm::vec2(0.0f, 1.0f)));

	std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0,
									5, 4, 7, 7, 6 ,5,
									1, 5, 6, 6, 2, 1,
									4, 0, 3, 3, 7, 4,
									3, 2, 6, 6, 7, 3,
									0, 1, 5, 5, 4, 0 };

	gls::Texture diffuse1;
	gls::load_texture(diffuse1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_diffuse.png", "texture_diffuse");

	gls::Texture specular1;
	gls::load_texture(specular1, "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/textures/container_specular.png", "texture_specular");

	std::vector<gls::Texture> textures = { diffuse1, specular1 };

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

void voxelize_scene(const gls::Model & m, const gls::Shader & vs, GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, voxel_grid_width, voxel_grid_height);
	
	//Disable some fixed-function opeartions
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	
	cgs::Transform obj_transf;
	obj_transf.set_object_to_upright(cgm::scale(0.005f, 0.005f, 0.005f));
	obj_transf.set_position(cgm::vec3(0.0f, 0.0f, 0.0f));
	

	cgs::Camera camera;
	camera.set_mode(cgs::Camera::ORTHOGRAPHIC);
	camera.scale_film_gate(voxel_grid_width, voxel_grid_height);
	
	
	/// ORTHOGONAL PROJECTION ALONG THE X AXIS ///////////////////
	//cgm::mat4 mvp_x = cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	//mvp_x.concat_assign(cgm::translate(cgm::vec3(2.0f, 0.0f, 0.0f)));
	//mvp_x = cgm::invert_orthogonal(mvp_x);

	camera.get_transform().set_object_to_upright(cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), 90.0f));
	camera.get_transform().set_position(cgm::vec3(2.0f, 0.0f, 0.0f));
	
	cgm::mat4 mvp_x = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_x.concat_assign(camera.get_projection());
	GLint mvp_x_loc =  vs.get_uniform_location("u_mvp_x");
	glUniformMatrix4fv(mvp_x_loc, 1, GL_FALSE, mvp_x.value_ptr());

	/// -------------------------------------/////////////////////
	
	/// ORTHOGONAL PROJECTION ALONG THE Y AXIS //////
	//cgm::mat4 mvp_y = cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	//mvp_y.concat_assign(cgm::translate(cgm::vec3(0.0f, 2.0f, 0.0f)));
	//mvp_y = cgm::invert_orthogonal(mvp_y);
	
	camera.get_transform().set_object_to_upright(cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), -90.0f));
	camera.get_transform().set_position(cgm::vec3(0.0f, 2.0f, 0.0f));

	cgm::mat4 mvp_y = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_y.concat_assign(camera.get_projection());
	GLint mvp_y_loc = vs.get_uniform_location("u_mvp_y");
	glUniformMatrix4fv(mvp_y_loc, 1, GL_FALSE, mvp_y.value_ptr());
	//-----------------------------------------/////
	
	// ORTHOGONAL PROJECTION ALONG THE Z AXIS //
	//cgm::mat4 mvp_z = cgm::translate(cgm::vec3(0.0f, 0.0f, 2.0f));
	//mvp_z = cgm::invert_orthogonal(mvp_z);

	camera.get_transform().set_object_to_upright(cgm::mat4());
	camera.get_transform().set_position(cgm::vec3(0.0f, 0.0f, 2.0f));

	cgm::mat4 mvp_z = cgm::concat_mat4(obj_transf.object_to_world(), cgm::invert_orthogonal(camera.get_transform().object_to_world()));
	mvp_z.concat_assign(camera.get_projection());
	GLint mvp_z_loc = vs.get_uniform_location("u_mvp_z");
	glUniformMatrix4fv(mvp_z_loc, 1, GL_FALSE, mvp_z.value_ptr());
	// --------------------------------------//
	
	glUniform1i(vs.get_uniform_location("u_voxel_grid_width"), voxel_grid_width);
	glUniform1i(vs.get_uniform_location("u_voxel_grid_height"), voxel_grid_height);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vs.use();
		m.render(vs);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
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
	
	//glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);

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
	
	voxelize_scene(model_suit ,voxelize_shader, window);
	std::cout << "finish" << std::endl;
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