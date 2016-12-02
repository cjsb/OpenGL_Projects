//
//  vec3.hpp
//  
//
//  Created by Mateus on 15/06/16.
//
//

#ifndef _VEC3_HPP

#define _VEC3_HPP
#include <iostream>
namespace cgm {
    struct vec3 {
        explicit vec3(float x = 0.0, float y = 0.0f, float z = 0.0f);
        vec3(const vec3 & v);
     
    
        const float * value_ptr()  const {return &this -> x; }
              vec3  & negate();
              vec3  & add_assign(const vec3 & rhs);
              vec3  & sub_assign(const vec3 & rhs);
              vec3  & scale(float k);
              float   magnitude() const;
              vec3  & normalize();
		
			  float x, y, z;
    };
    std::ostream & print_vec3(std::ostream & os, const vec3 & rhs);
}
#endif