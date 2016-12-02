//
//  vector_operations.hpp
//  
//
//  Created by Mateus on 28/06/16.
//
//

#ifndef _VECTOR_OPERATIONS_HPP

#define _VECTOR_OPERATIONS_HPP
#include "vec3.hpp"
namespace cgm {
    vec3   add(const vec3 & lhs, const vec3 & rhs);
    vec3   sub(const vec3 & lhs, const vec3 & rhs);
    vec3   scale(const vec3 & v, float k);
    float  magnitude(const vec3 & v);
    vec3   normalize(const vec3 & v);
    float  dot(const vec3 & lhs, const vec3 & rhs);
    vec3   cross(const vec3 & lhs, const vec3 & rhs);
}

#endif