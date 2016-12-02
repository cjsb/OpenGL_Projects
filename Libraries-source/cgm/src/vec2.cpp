#include <iostream>
#include "vec2.hpp"

std::ostream & cgm::print_vec2(std::ostream & os, const cgm::vec2 & v) 
{
	os << "[ " << v.x << ", " << v.y << "]";

	return os;
}