//
//  matrix_transform.cpp
//  
//
//  Created by Mateus on 16/06/16.
//
//

#define _USE_MATH_DEFINES
#include "matrix_transform.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include <cmath>



cgm::mat4 cgm::frustum(float l, float r, float b, float t, float n, float f)
{
    cgm::mat4 perspective;
    
    perspective.elements[0][0] = 2 * n / (r - l); perspective.elements[0][1] = 0.0f; perspective.elements[0][2] = 0.0f; perspective.elements[0][3] = 0.0f;
    perspective.elements[1][0] = 0.0f; perspective.elements[1][1] = 2 * n / (t - b); perspective.elements[1][2] = 0.0f; perspective.elements[1][3] = 0.0f;
    perspective.elements[2][0] = (r + l) /(r -l); perspective.elements[2][1] = (t + b)/(t - b); perspective.elements[2][2] = -(f + n)/(f - n); perspective.elements[2][3] = -1.0f;
    perspective.elements[3][0] = 0.0f; perspective.elements[3][1] = 0.0f; perspective.elements[3][2] = (-2 * f * n) / (f - n); perspective.elements[3][3] = 0.0f;
    
    return perspective;
}

cgm::mat4 cgm::ortho(float l, float r, float b, float t, float n, float f)
{
    cgm::mat4 orthographic;
    
    orthographic.elements[0][0] = 2.0f / (r - l); orthographic.elements[0][1] = 0.0f; orthographic.elements[0][2] = 0.0f; orthographic.elements[0][3] = 0.0f;
    orthographic.elements[1][0] = 0.0f; orthographic.elements[1][1] = 2.0f/ (t - b); orthographic.elements[1][2] = 0.0f; orthographic.elements[1][3] = 0.0f;
    orthographic.elements[2][0] = 0.0f; orthographic.elements[2][1] = 0.0f; orthographic.elements[2][2] = -2 / (f - n); orthographic.elements[2][3] = 0.0f;
    orthographic.elements[3][0] = -(r + l)/(r - l); orthographic.elements[3][1] = -(t + b) / (t - b); orthographic.elements[3][2] = -(f + n) / (f - n); orthographic.elements[3][3] = 1.0f;
    
    return orthographic;
}


cgm::mat4 cgm::translate(const cgm::vec3 & t)
{
    cgm::mat4 translate_matrix(cgm::vec3(1.0f, 0.0f, 0.0f), cgm::vec3(0.0f, 1.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), t);
    
    return translate_matrix;
}

cgm::mat4 cgm::rotate(const cgm::vec3 & n, float theta)
{
    cgm::vec3 p, q, r;
    
    theta *= M_PI / 180.0f;
    
    p.x = pow(n.x, 2.0f) * (1.0f - cos(theta)) + cos(theta);
    p.y = n.x * n.y * (1.0f - cos(theta)) + n.z * sin(theta);
    p.z = n.x * n.z * (1.0f - cos(theta)) - n.y * sin(theta);
    
    q.x = n.x * n.y * (1.0f - cos(theta)) - n.z * sin(theta);
    q.y = pow(n.y, 2.0f) * (1.0f - cos(theta)) + cos(theta);
    q.z = n.y * n.z * (1.0f - cos(theta)) + n.x * sin(theta);
    
    r.x = n.x * n.z * (1.0f - cos(theta)) + n.y * sin(theta);
    r.y = n.y * n.z * (1.0f - cos(theta)) - n.x * sin(theta);
    r.z = pow(n.z, 2.0f) * (1.0f - cos(theta)) + cos(theta);
    
    cgm::mat4 rotation_mat(p, q, r, cgm::vec3());
    
    return rotation_mat;
}


cgm::mat4 cgm::invert_orthogonal(const cgm::mat4 & a)
{
    cgm::vec3 p, q, r, t;
    
    p = cgm::vec3(a.elements[0][0], a.elements[0][1], a.elements[0][2]);
    q = cgm::vec3(a.elements[1][0], a.elements[1][1], a.elements[1][2]);
    r = cgm::vec3(a.elements[2][0], a.elements[2][1], a.elements[2][2]);
    t = cgm::vec3(a.elements[3][0], a.elements[3][1], a.elements[3][2]);
    
    cgm::mat4 linear, affine, inverse;
    
    // get the linear portion and transpose it, i.e invert it
    linear = cgm::transpose_mat4( cgm::mat4(p, q, r, cgm::vec3() ) );
    // get the tranlation portion and invert it
    affine = cgm::mat4(cgm::vec3(1.0f, 0.0f, 0.0f), cgm::vec3(0.0f, 1.0f, 0.0f), cgm::vec3(0.0f, 0.0f, 1.0f), t.negate());
    
    inverse = cgm::concat_mat4(affine, linear);
    
    return inverse;
}

cgm::mat4 cgm::scale(const float kx, const float ky, const float kz)
{
    return cgm::mat4(cgm::vec3(kx, 0.0f, 0.0f), cgm::vec3(0.0f, ky, 0.0f), cgm::vec3(0.0f, 0.0f, kz), cgm::vec3());
}
