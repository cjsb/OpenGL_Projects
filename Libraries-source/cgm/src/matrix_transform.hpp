//
//  matrix_transform.hpp
//  
//
//  Created by Mateus on 16/06/16.
//
//

#ifndef _MATRIX_TRANSFORM_HPP

#define _MATRIX_TRANSFORM_HPP
#include "mat4.hpp"
#include "vec3.hpp"

namespace cgm {
    void compute_fov(float film_size_w, float film_size_h, float focal_length, float & fov_h, float & fov_v);
    void compute_bnd(float fov_h, float fov_v, float near, float & left, float & right, float & bottom, float & top);
    mat4 frustum(float l, float r, float b, float t, float n, float f);
    mat4 ortho(float l, float r, float b, float t, float n, float f);
    mat4 translate(const cgm::vec3 & t);
    mat4 invert_orthogonal(const mat4 & a);
    mat4 rotate(const vec3 & n, float theta);
    mat4 scale(const float kx, const float ky, const float kz);
}
#endif