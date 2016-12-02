//
//  vec3.cpp
//  
//
//  Created by Mateus on 15/06/16.
//
//

#include "vec3.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

cgm::vec3::vec3(float x, float y, float z)
{
    this -> x = x;
    this -> y = y;
    this -> z = z;
}

cgm::vec3::vec3(const cgm::vec3 & v)
{
    *this = v;
}

cgm::vec3 & cgm::vec3::negate()
{
    x *= -1.0f;
    y *= -1.0f;
    z *= -1.0f;
    
    return *this;
}

cgm::vec3 & cgm::vec3::add_assign(const cgm::vec3 & rhs)
{
	x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    
    return *this;
}

cgm::vec3 & cgm::vec3::sub_assign(const cgm::vec3 & rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    
    return *this;
}

cgm::vec3 & cgm::vec3::scale(float k)
{
    x *= k;
    y *= k;
    z *= k;
    
    return *this;
}

float cgm::vec3::magnitude() const
{
    float m ;
    
    m = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    
    return m;
}

cgm::vec3 & cgm::vec3::normalize()
{
    float k = magnitude();
    
    if (k != 0) {
        scale(1.0f/k);
    }
    return *this;
    
    
}

std::ostream & cgm::print_vec3(std::ostream & os, const cgm::vec3 & rhs)
{
    int field_width = 5;
    
    os << " --" <<  std::setw(field_width * 2 + 3) << std::right << "--" << std::endl;
    os << "|_ " <<  std::setw(field_width) << std::left << rhs.x << std::setw(field_width) << std::left  << rhs.y  << rhs.z << " _|" << std::endl;
    
    return os;
}