//
//  Camera.cpp
//  
//
//  Created by Mateus_Gondim on 27/06/16.
//
//
#include "Camera.hpp"
#include "matrix_transform.hpp"
#include <cmath>

cgs::Camera::Camera()
{
    film_aspect_ratio    =  film_width / film_height;
    device_aspect_ratio  =  image_width / (float)image_height;
    compute_fov();
    compute_bnd();
    compute_proj_mat();
}

void cgs::Camera::compute_fov()
{
    float inch_to_mm = 25.4f;
    float film_size_w, film_size_h;
    
    film_size_w  = film_width  * inch_to_mm;
    film_size_h  = film_height * inch_to_mm;
    
    fov_h = 2.0f * atan( film_size_w / ( 2.0f * focal_length) );
    fov_v = 2.0f * atan( film_size_h / ( 2.0f * focal_length) );
}

void cgs::Camera::compute_bnd()
{
    right = near * tan( fov_h / 2.0f);
    left = - right;
    
    top = near * tan( fov_v / 2.0f );
    bottom = - top;
}

void cgs::Camera::scale_film_gate(int new_image_width, int new_image_height)
{
    float x_scale, y_scale;
    
    x_scale = y_scale = 1.0f;
    
    if ( (image_width != new_image_width) || (image_height != new_image_height) ) {
        compute_bnd();
        
        image_width  = new_image_width;
        image_height = new_image_height;
        device_aspect_ratio = image_width / (float)image_height;
    
        if (film_aspect_ratio > device_aspect_ratio) {
            y_scale = film_aspect_ratio / device_aspect_ratio;
        }
        else if (film_aspect_ratio < device_aspect_ratio) {
            x_scale = device_aspect_ratio / film_aspect_ratio;
        }
        
        right *= x_scale;
        top *= y_scale;
        
        left = -right;
        bottom = -top;
        
        compute_proj_mat();
    }
}

void cgs::Camera::compute_proj_mat()
{
    if (mode == PERSPECTIVE) {
        projection = cgm::frustum(left, right, bottom, top, near, far);
    }
    else {
        projection = cgm::ortho(left, right, bottom, top, near, far);
    }
}

void cgs::Camera::set_mode(cgs::Camera::Projection_type m)
{
    if (mode != m) {
        mode = m;
        compute_proj_mat();
    }
}


void cgs::Camera::get_bnd(float & l, float & r, float & b, float & t, float & n, float & f) const
{
    l = left;
    r = right;
    b = bottom;
    t = top;
    n = near;
    f = far;
}
