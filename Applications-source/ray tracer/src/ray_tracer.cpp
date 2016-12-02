//
//  ray_tracer.cpp
//  
//
//  Created by Mateus Gondim on 9/8/16.
//
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "matrix_transform.hpp"
#include "Camera.hpp"
#include "vec3.hpp"
#include "vector_operations.hpp"
#include "mat4.hpp"
#include "ray_tracer.hpp"
#include <iostream>

/*  TODO : Add command line options for number of spheres, radius of spheres
 *         type of BRDF, light options , type of microfacet and type of integrator
 *
 */
using std::endl; 
using std::cerr;
 
GLfloat vertices[4][3] = {{-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f},
                          {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}
                          };
GLuint  indices[6] = {0, 1, 2, 2, 3, 0};


bool keys[1024];

cgm::vec3   fst_sphere_pos(2.0f,0.0f, -7.0f);
cgm::vec3   fst_sphere_color(1.0f, 0.0f, 0.0f);
float       fst_sphere_radius = 1.0f;

cgm::vec3   snd_sphere_pos(0.0f, 0.0f, -5.0f);
cgm::vec3   snd_sphere_color(0.0f, 0.0f, 1.0f);
float       snd_sphere_radius = 1.0f;

cgm::vec3 light_pos(-5.0f, 0.0f, -4.0f);

float curr_time, prev_time = 0.0f, delta_time;
float cam_speed = 1.6f;

void set_up(GLuint & vao, GLuint & vbo, GLuint & ebo, const Shader & shader)
{

    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    shader.use();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLint vertex_pos_loc =  glGetAttribLocation(shader.get_program(), "vertex_position");
    
    if (vertex_pos_loc == -1) {
        cerr << "ERROR: attribute 'vertex_position' not found" << endl;
    }
    
    glVertexAttribPointer(vertex_pos_loc , 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertex_pos_loc);
    
    glBindVertexArray(0);
}



void error_callback(int error, const char *description)
{
    cerr << description << endl;
    
}

void key_callback(GLFWwindow * window, int key, int scancode, int action , int mods)
{
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

int main(int argc, char *argv[])
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        exit(-1);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(640, 480, "Ray Tracer", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
	
	glewExperimental = GL_TRUE;
	glewInit();
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    GLuint vao, vbo, ebo;
	std::cerr << "ERROROROROROROROROROR" << std::endl;
	
	Shader shader = Shader("C:/Users/mateu/Documents/Projects/Applications/ray tracer/source/shaders/ray_tracer.vert", "C:/Users/mateu/Documents/Projects/Applications/ray tracer/source/shaders/ray_tracer.frag");
	std::cerr << "ERROROROROROROROROROR" << std::endl;
	set_up(vao, vbo, ebo, shader);
    
    glEnable(GL_DEPTH_TEST);
    
    cgs::Camera camera;
    GLint model_loc, view_loc, projection_loc;
    
    model_loc       =  glGetUniformLocation(shader.get_program(), "model");
    view_loc        =  glGetUniformLocation(shader.get_program(), "view");
    projection_loc  =  glGetUniformLocation(shader.get_program(), "projection");
    
    if (model_loc == -1) {
        cerr << "ERROR: Could not find uniform 'model' " << endl;
    }
    if (view_loc == -1 ) {
        cerr << "ERROR: Could not find uniform 'view' " << endl;
    }
    if (projection_loc == -1 ) {
        cerr << "ERROR: Could not find uniform 'projection' " << endl;
    }
    float left, right, bottom, top, near, far;
    
    camera.get_bnd(left, right, bottom, top, near, far);
    
    cgm::mat4 model = cgm::scale(right * 2.0f, top * 2.0f, 1.0f);
    //cgm::print_mat4(cerr, model) << endl;
    model.concat_assign(cgm::translate( cgm::vec3(0.0f, 0.0f, -near) ) );
    model.concat_assign(camera.get_transform().object_to_upright());
    model.concat_assign(cgm::translate(camera.get_transform().get_position()));
    
    cgm::mat4 view = (camera.get_transform()).object_to_world();
    view = cgm::invert_orthogonal(view);
    
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
    glUniformMatrix4fv(view_loc, 1, GL_FALSE,  view.value_ptr());
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
    
    
    GLint cam_pos_loc     =  glGetUniformLocation(shader.get_program(), "cam_wld_pos");
    
    GLint fst_sphere_pos_loc    =  glGetUniformLocation(shader.get_program(), "fst_sphere_pos");
    GLint fst_sphere_r_loc      =  glGetUniformLocation(shader.get_program(), "fst_sphere_radius");
    GLint fst_sphere_color_loc  =  glGetUniformLocation(shader.get_program(), "fst_sphere_color");
    
    GLint snd_sphere_pos_loc    =  glGetUniformLocation(shader.get_program(), "snd_sphere_pos");
    GLint snd_sphere_r_loc      =  glGetUniformLocation(shader.get_program(), "snd_sphere_radius");
    GLint snd_sphere_color_loc  =  glGetUniformLocation(shader.get_program(), "snd_sphere_color");
    
    
    GLint light_pos_loc   =  glGetUniformLocation(shader.get_program(), "light_pos");
    
    if (cam_pos_loc == -1) {
        cerr << "ERROR: Could not find uniform 'cam_wld_pos'" << endl;
    }
    
    if (fst_sphere_pos_loc == -1){
        cerr << "ERROR: Could not find uniform 'fst_sphere_pos' " << endl;
    }
    if (fst_sphere_r_loc == -1) {
        cerr << "ERROR: Could not find uniform 'fst_sphere_radius' " << endl;
    }
    if (fst_sphere_color_loc == -1) {
        cerr << "ERROR: Could not find uniform 'fst_sphere_color' " << endl;
    }
    
    if (snd_sphere_pos_loc == -1){
        cerr << "ERROR: Could not find uniform 'snd_sphere_pos' " << endl;
    }
    if (snd_sphere_r_loc == -1) {
        cerr << "ERROR: Could not find uniform 'snd_sphere_radius' " << endl;
    }
    if (snd_sphere_color_loc == -1) {
        cerr << "ERROR: Could not find uniform 'snd_sphere_color' " << endl;
    }
    
    if (light_pos_loc == -1) {
        cerr << "ERROR: Could not find uniform 'light_pos' " << endl;
    }
    
    glUniform3fv(fst_sphere_pos_loc, 1, fst_sphere_pos.value_ptr());
    glUniform1f(fst_sphere_r_loc, fst_sphere_radius);
    glUniform3fv(fst_sphere_color_loc, 1, fst_sphere_color.value_ptr());
    
    glUniform3fv(snd_sphere_pos_loc, 1, snd_sphere_pos.value_ptr());
    glUniform1f(snd_sphere_r_loc, snd_sphere_radius);
    glUniform3fv(snd_sphere_color_loc, 1, snd_sphere_color.value_ptr());
    
    glUniform3fv(cam_pos_loc, 1, camera.get_transform().get_position().value_ptr());
    glUniform3fv(light_pos_loc, 1, light_pos.value_ptr());
    
    int curr_width, curr_height;
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &curr_width, &curr_height);
        glViewport(0, 0, curr_width, curr_height);
        
        if ( (camera.get_image_width() != curr_width) || (camera.get_image_height() != curr_height) ) {
            camera.scale_film_gate(curr_width, curr_height);
            
            camera.get_bnd(left, right, bottom, top, near, far);
            model = cgm::scale(2.0f * right, 2.0f * top, 1.0f);
            model.concat_assign(cgm::translate( cgm::vec3(0.0f, 0.0f, -near) ) );
            model.concat_assign(camera.get_transform().object_to_upright());
            model.concat_assign(cgm::translate(camera.get_transform().get_position()));

            
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
            cerr << "scaling film gate " << endl;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //shader.use()
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        curr_time   =  glfwGetTime();
        delta_time  =  curr_time - prev_time;
        prev_time   =  curr_time;
        
        cgm::mat4 orientation = camera.get_transform().object_to_upright();
        cgm::vec3 p = cgm::normalize(orientation.p());
        cgm::vec3 q = cgm::normalize(orientation.q());
        cgm::vec3 r = cgm::normalize(orientation.r());
        
        if (keys[GLFW_KEY_W] == true){
            camera.get_transform().translate( cgm::scale ( cgm::scale(r, -1.0f) , (cam_speed * delta_time)));
        }
        if(keys[GLFW_KEY_S] == true) {
            camera.get_transform().translate( cgm::scale ( r, (cam_speed * delta_time)));
        }
        if(keys[GLFW_KEY_A] == true){
        camera.get_transform().translate( cgm::scale (cgm::scale(p, -1.0f) , (cam_speed * delta_time)));
        }
        if(keys[GLFW_KEY_D] == true){
        camera.get_transform().translate( cgm::scale (p , (cam_speed * delta_time)));
        }
        
        model = cgm::scale(2.0f * right, 2.0f * top, 1.0f);
        model.concat_assign(cgm::translate( cgm::vec3(0.0f, 0.0f, -near) ) );
        model.concat_assign(camera.get_transform().object_to_upright());
        model.concat_assign(cgm::translate(camera.get_transform().get_position()));
        
        view = camera.get_transform().object_to_world();
        view = cgm::invert_orthogonal(view);
        
        glUniform3fv(cam_pos_loc, 1, camera.get_transform().get_position().value_ptr());
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.value_ptr());
        glUniformMatrix4fv(view_loc, 1, GL_FALSE,  view.value_ptr());
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
    
}