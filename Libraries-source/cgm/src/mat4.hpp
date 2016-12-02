//
//  mat4.hpp
//  
//
//  Created by Mateus on 16/06/16.
//
//

#ifndef _MAT4_HPP

#define _MAT4_HPP
#include <iostream>
#include "vec3.hpp"

namespace cgm {
    class mat4 {
    friend std::ostream &print_mat4(std::ostream & os, const mat4 & rhs);
    friend mat4 frustum(float l, float r, float b, float t, float n, float f);
    friend mat4 ortho(float l, float r, float b, float t, float n, float f);
    friend mat4 invert_orthogonal(const mat4 & a);
    public:
        mat4();
        mat4(const mat4 & m) { *this = m; }
        mat4(float el_00, float el_01, float el_02, float el_03,
             float el_10, float el_11, float el_12, float el_13,
             float el_20, float el_21, float el_22, float el_23,
             float el_30, float el_31, float el_32, float el_33);
        mat4(const cgm::vec3 & p, const cgm::vec3 & q, const cgm::vec3 & r, const cgm::vec3 & t);
        
        mat4 & concat_assign(const mat4 & rhs);
        mat4 & transpose();
        const float *value_ptr() const  { return &elements[0][0]; }
        vec3 p() const ;
        vec3 q() const ;
        vec3 r() const ;
    private:
        float elements[4][4];
    };
    
    mat4 concat_mat4(const mat4 & lhs , const mat4 & rhs);
    mat4 transpose_mat4(const mat4 & a);
    
    std::ostream &print_mat4(std::ostream & os, const mat4 & rhs);
}


#endif