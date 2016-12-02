//
//  test.cpp
//
//
//  Created by Mateus_Gondim on 01/06/16.
//
//

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "SOIL.h"
#include "test.hpp"
#include "Shader.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "matrix_transform.hpp"
#include "vector_operations.hpp"
#include "Camera.hpp"
#include "Transform.hpp"

using std::cerr;
using std::endl;
using std::cout;


GLfloat vertices[8][3] = { {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
                           {0.5f, 0.5f, -0.5f},   {-0.5f, 0.5f, -0.5f},
                           {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
                           {0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, 0.5f},};

GLuint indices[36] = {0, 1, 2, 2, 3, 0,
                      5, 4, 7, 7, 6 ,5,
                      1, 5, 6, 6, 2, 1,
                      4, 0, 3, 3, 7, 4,
                      3, 2, 6, 6, 7, 3,
                      0, 1, 5, 5, 4, 0};

GLfloat text_coords[8][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
                             {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}};

float cam_speed = 4.0f;
cgm::vec3 cam_pos(0.0f,0.0f,2.0f);
bool keys[1024];

float delta_time;
float curr_time = glfwGetTime();
float prev_time = 0.0f;

float yaw_theta = 0.0f;
float pitch_theta = 0.0f;
double curr_mouse_x_pos = 0;
double last_mouse_x_pos = 0;
double curr_mouse_y_pos = 0;
double last_mouse_y_pos = 0;
double mouse_sensitivity = 0.5;


cgm::vec3 cam_right(1.0f, 0.0f, 0.0f);
cgm::vec3 cam_up(0.0f, 1.0f, 0.0f);
cgm::vec3 cam_forward(0.0f,0.0f,1.0f);

void initBuffers(GLuint & vao, GLuint & vbo, GLuint & element_buffer, GLuint & text_coord_buff, GLuint & text_id, const Shader & shader )
{
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &text_coord_buff);
    glBindBuffer(GL_ARRAY_BUFFER, text_coord_buff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(text_coords), text_coords, GL_STATIC_DRAW);
    
    
    shader.use();
    
    glBindBuffer(GL_ARRAY_BUFFER ,vbo);
    GLint vert_pos = glGetAttribLocation(shader.get_program(), "vert_pos");
    glVertexAttribPointer(vert_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vert_pos);
    
    glBindBuffer(GL_ARRAY_BUFFER, text_coord_buff);
    GLint vert_text_coord = glGetAttribLocation(shader.get_program(), "vert_text_coord");
    glVertexAttribPointer(vert_text_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vert_text_coord);
    
    int width, height;
	unsigned char *image = SOIL_load_image("C:/Users/mateu/Documents/Projects/Applications/test/source/textures/wood_container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    
    glGenTextures(1, &text_id);
    glBindTexture(GL_TEXTURE_2D, text_id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    glBindTexture( GL_TEXTURE_2D, 0);
    
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

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (curr_mouse_x_pos != xpos) {
        curr_mouse_x_pos = xpos;
        yaw_theta -= (curr_mouse_x_pos - last_mouse_x_pos) * mouse_sensitivity;
        last_mouse_x_pos = curr_mouse_x_pos;
    }
   
    if (curr_mouse_y_pos != ypos) {
        curr_mouse_y_pos = ypos;
        pitch_theta -= (curr_mouse_y_pos - last_mouse_y_pos) * mouse_sensitivity;
        pitch_theta =  (pitch_theta >= 0) ?(fmin(pitch_theta, 89.0f)) :(fmax(pitch_theta, -89.0f));
        last_mouse_y_pos = curr_mouse_y_pos;
    }
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
    
    GLFWwindow *window = glfwCreateWindow(640, 480, "test", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);
   
	glewExperimental = GL_TRUE;
	glewInit();
    
    glEnable(GL_DEPTH_TEST);
    
    GLuint vao, vbo, element_buffer, text_coord_buff, text_id;
    
    Shader shader("C:/Users/mateu/Documents/Projects/Applications/test/source/shaders/vertex_shader.vert", "C:/Users/mateu/Documents/Projects/Applications/test/source/shaders/fragment_shader.frag");
    
    
    initBuffers(vao, vbo, element_buffer, text_coord_buff, text_id, shader);
    
    GLint view_loc = glGetUniformLocation(shader.get_program(), "view");
    GLint proj_loc = glGetUniformLocation(shader.get_program(), "projection");
    
///////// TESTING VECTORS AND MATRIXES /////////////////////////////////////////////////////////////////
    cgm::mat4 model = cgm::translate(cgm::vec3(0.0f, 0.0f,0.0f));
    cgm::mat4 cam   = cgm::translate(cam_pos);
    
    cgm::mat4 orientation = cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f ),  yaw_theta);
    orientation.concat_assign(cgm::rotate(orientation.p(), pitch_theta));
    
    
    cgm::mat4 view  = cgm::concat_mat4(model, cgm::invert_orthogonal(concat_mat4(orientation, cam)));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());
    
    cgm::mat4 projection;
    
    cgs::Camera camera;
    
    float film_width, film_height, focal_lenght, film_aspect_ratio;
    float fov_h, fov_v;
    float left,right,bottom, top, near, far;
    
    //35mm
    focal_lenght =  35;
    film_width   =  0.980f;
    film_height  =  0.735f;
    film_aspect_ratio = film_width / film_height;
    near = 0.1f;
    far  = 100;
    
    //cgm::compute_fov(film_width, film_height, focal_lenght, fov_h, fov_v);
    //cgm::compute_bnd(fov_h, fov_v, near, left, right, bottom, top);
    
////////////////////////////////////////////////////////////////////////////////////////////////////
    int curr_width, curr_height;
    
    while (!glfwWindowShouldClose(window)) {
        
        float device_aspect_ratio;
        

        glfwGetFramebufferSize(window, &curr_width, &curr_height);
        glViewport(0, 0, curr_width, curr_height);
        
        if ( (camera.get_image_width() != curr_width) || (camera.get_image_height() != curr_height) ) {
            camera.scale_film_gate(curr_width, curr_height);
            //device_aspect_ratio = curr_width / curr_height;
            
            //cgm::scale_film_gate(left, right, bottom, top, film_aspect_ratio, device_aspect_ratio);
            //projection = cgm::frustum(left, right, bottom, top, near, far);
            
            //cgm::print_mat4(cout, projection);
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
        }
        
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, camera.get_projection().value_ptr());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
       
        glBindTexture(GL_TEXTURE_2D, text_id);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        curr_time = glfwGetTime();
        delta_time = curr_time - prev_time;
        prev_time = curr_time;
        
        
        cam_right = orientation.p();
        cam_up    = orientation.q();
        cam_forward = orientation.r();
        
        cam_right.normalize();
        cam_up.normalize();
        cam_forward.normalize();
        
        cgm::vec3 p = cam_right;
        cgm::vec3 r = cam_forward;
        
        p.scale(cam_speed * delta_time);
        r.scale(cam_speed * delta_time);
        
        if (keys[GLFW_KEY_W] == true) {
            cam_pos = cgm::sub(cam_pos,r);
        }
         if (keys[GLFW_KEY_S] == true) {
             cam_pos = cgm::add(cam_pos, r);
        }
         if (keys[GLFW_KEY_A] == true ) {
             cam_pos = cgm::sub(cam_pos, p);
        }
         if (keys[GLFW_KEY_D] == true) {
             cam_pos = cgm::add(cam_pos, p);
        }
        if (keys[GLFW_KEY_Q] == true) {
            break;
        }
        
        cam   = cgm::translate(cam_pos);
        
        orientation = cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f ),  yaw_theta);
        orientation.concat_assign(cgm::rotate(orientation.p(), pitch_theta));
        
        view  = cgm::concat_mat4(model, cgm::invert_orthogonal(concat_mat4(orientation, cam)));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.value_ptr());
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
