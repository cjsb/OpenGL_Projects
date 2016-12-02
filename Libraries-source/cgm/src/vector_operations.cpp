//
//  vector_operations.cpp
//  
//
//  Created by Mateus on 28/06/16.
//
//

#include "vector_operations.hpp"
#include "vec3.hpp"
#include <cmath>

cgm::vec3 cgm::add(const cgm::vec3 & lhs, const cgm::vec3 & rhs)
{
    cgm::vec3 r = lhs;
    
    r.x += rhs.x;
    r.y += rhs.y;
    r.z += rhs.z;
    
    return r;
}

cgm::vec3 cgm::sub(const cgm::vec3 & lhs, const cgm::vec3 & rhs)
{
    cgm::vec3 r = lhs;
    
    r.x -= rhs.x;
    r.y -= rhs.y;
    r.z -= rhs.z;
    
    return r;
}

cgm::vec3 cgm::scale(const cgm::vec3 & v, float k)
{
    cgm::vec3 r = v;
    
    r.x *= k;
    r.y *= k;
    r.z *= k;
    
    return r;
}

float cgm::dot(const cgm::vec3 & lhs, const cgm::vec3 & rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

cgm::vec3 cgm::cross(const cgm::vec3 & lhs, const cgm::vec3 & rhs)
{
    cgm::vec3 r;
    
    r.x = lhs.y * rhs.z - lhs.z * rhs.y;
    r.y = lhs.z * rhs.x - lhs.x * rhs.z;
    r.z = lhs.x * rhs.y - lhs.y * rhs.x;
    
    return r;
}

float cgm::magnitude(const cgm::vec3 & v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

cgm::vec3 cgm::normalize(const cgm::vec3 & v)
{
    cgm::vec3 r = v;
    float k = magnitude(r);
    
    return scale(r, 1 / k);
}