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
#include "mat4.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

void error_callback(int error, const char *description)
{
	std::cerr << description << std::endl;

}

gls::Mesh setup_box()
{
	/*GLfloat vertices[8][3] = { { -0.5f, -0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
	{ 0.5f, 0.5f, -0.5f },{ -0.5f, 0.5f, -0.5f },
	{ -0.5f, -0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },
	{ 0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f }, };*/

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
	/*GLuint indices[36] = { 0, 1, 2, 2, 3, 0,
		5, 4, 7, 7, 6 ,5,
		1, 5, 6, 6, 2, 1,
		4, 0, 3, 3, 7, 4,
		3, 2, 6, 6, 7, 3,
		0, 1, 5, 5, 4, 0 };

	GLfloat text_coords[8][2] = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
	{ 1.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };*/
}

int main(int argc, char *argv[]) 
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Model Loader", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	//glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	//Initialize GLEW 
	glewExperimental = GL_TRUE;
	glewInit();

	//gls::Mesh mesh = setup_box();
	

	gls::Shader shader("../../Applications-source/model_loader/shaders/vertex.vert", "../../Applications-source/model_loader/shaders/fragment.frag");
//	gls::Shader shader("C:/Users/mateu/Documents/Projects/Applications/model_loader/source/shaders/vertex2.vert", "C:/Users/mateu/Documents/Projects/Applications/model_loader/source/shaders/fragment2.frag");

	//mesh.do_shader_plumbing(shader, "v_position", "v_normal", "v_uv");
	shader.use();
	gls::Model model_suit("../../Resources/nanosuit/nanosuit.obj");
	//model_suit.do_shader_plumbing(shader, "v_position", "v_normal", "v_uv");
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	cgm::vec3 light_color(1.0f, 1.0f, 1.0f);
	GLint light_color_loc = glGetUniformLocation(shader.get_program(), "light_color");
	if (light_color_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'light_color' in shader program" << std::endl;
	}

	cgm::vec3 light_pos(0.0f, 8.0f, 0.0f);
	GLint light_pos_loc = glGetUniformLocation(shader.get_program(), "light_pos");
	if (light_pos_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'light_pos' in shader program" << std::endl;
	}

	GLint cam_pos_loc = glGetUniformLocation(shader.get_program(), "cam_pos");
	if (cam_pos_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'cam_pos' in shader program" << std::endl;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//set matrixes
	cgs::Transform model_transform;
	model_transform.set_position(cgm::vec3(0.0f, -1.75f, -5.8f));
	cgs::Camera camera;
	cgm::mat4  aux = cgm::scale(0.2f, 0.2f, 0.2f);
	aux.concat_assign(cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), 45.0f));
	cgm::mat4   model = cgm::concat_mat4(aux, model_transform.object_to_world());
	cgm::mat4   view = cgm::invert_orthogonal(camera.get_transform().object_to_world());
	
	
	GLint model_loc = glGetUniformLocation(shader.get_program(), "model");
	if (model_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'model' in shader program" << std::endl;
	}
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
	
	GLint view_loc = glGetUniformLocation(shader.get_program(), "view");
	if (view_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'view' in shader program" << std::endl;
	}
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());

	GLint p_loc  = glGetUniformLocation(shader.get_program(), "projection");
	if (p_loc == -1) {
		std::cerr << "ERROR: Could not find uniform 'projection' in shader program" << std::endl;
	}

	//////////////////////
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

	
		//model = cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), theta );
		//model.concat_assign(cgm::translate(cgm::vec3(0.0f, 0.0f, -3.5f)));
		//glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
		//theta += 5.0f * delta_time;

		float device_aspect_ratio;

		glfwGetFramebufferSize(window, &curr_width, &curr_height);
		glViewport(0, 0, curr_width, curr_height);

		if ((camera.get_image_width() != curr_width) || (camera.get_image_height() != curr_height)) {
			camera.scale_film_gate(curr_width, curr_height);
			glUniformMatrix4fv(p_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
		}

		glUniformMatrix4fv(p_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
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