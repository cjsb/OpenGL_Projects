#ifndef _VEC2_HPP

#define _VEC2_HPP
#include <iostream>
//OBS: Different design in comparison to the vec3 class
namespace cgm {
	struct vec2 {
		explicit vec2(float x = 0, float y = 0) {
			this-> x = x;
			this-> y = y;
		}
		vec2(const vec2 & v) { *this = v; }
		
		const float *value_ptr() const { return &this -> x; }
		
		float x;
		float y;
	};

	std::ostream & print_vec2(std::ostream & os, const vec2 & v);
}

#endif 
