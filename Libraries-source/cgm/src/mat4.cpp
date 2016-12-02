//
//  mat4.cpp
//  
//
//  Created by Mateus on 16/06/16.
//
//

#include "mat4.hpp"
#include "vec3.hpp"
#include <iostream>
#include <iomanip>

using cgm::mat4;

cgm::mat4::mat4()
{
    elements[0][0] = 1.0f; elements[0][1] = 0.0f; elements[0][2] = 0.0f; elements[0][3] = 0.0f;
    elements[1][0] = 0.0f; elements[1][1] = 1.0f; elements[1][2] = 0.0f; elements[1][3] = 0.0f;
    elements[2][0] = 0.0f; elements[2][1] = 0.0f; elements[2][2] = 1.0f; elements[2][3] = 0.0f;
    elements[3][0] = 0.0f; elements[3][1] = 0.0f; elements[3][2] = 0.0f; elements[3][3] = 1.0f;
}

cgm::mat4::mat4(const vec3 & p, const vec3 & q, const vec3 & r, const vec3 & t)
{
    elements[0][0] = p.x; elements[0][1] = p.y; elements[0][2] = p.z; elements[0][3] = 0.0f;
    elements[1][0] = q.x; elements[1][1] = q.y; elements[1][2] = q.z; elements[1][3] = 0.0f;
    elements[2][0] = r.x; elements[2][1] = r.y; elements[2][2] = r.z; elements[2][3] = 0.0f;
    elements[3][0] = t.x; elements[3][1] = t.y; elements[3][2] = t.z; elements[3][3] = 1.0f;
}

mat4::mat4(float el_00, float el_01, float el_02, float el_03,
           float el_10, float el_11, float el_12, float el_13,
           float el_20, float el_21, float el_22, float el_23,
           float el_30, float el_31, float el_32, float el_33)
{
    elements[0][0] = el_00 ; elements[0][1] = el_01;  elements[0][2] =  el_02; elements[0][3] =  el_03;
    elements[1][0] = el_10;  elements[1][1] = el_11;  elements[1][2] =  el_12; elements[1][3] =  el_13;
    elements[2][0] = el_20;  elements[2][1] = el_21;  elements[2][2] =  el_22; elements[2][3] =  el_23;
    elements[3][0] = el_30;  elements[3][1] = el_31;  elements[3][2] =  el_32; elements[3][3] =  el_33;
}

cgm::vec3 mat4::p() const
{
    cgm::vec3 p(elements[0][0], elements[0][1], elements[0][2]);
    
    return p;
}

cgm::vec3 mat4::q() const
{
    cgm::vec3 q(elements[1][0], elements[1][1], elements[1][2]);
    
    return q;
}

cgm::vec3 mat4::r() const
{
    cgm::vec3 r(elements[2][0], elements[2][1], elements[2][2]);
    
    return r;
}


mat4 & mat4::concat_assign(const mat4 & rhs)
{
    mat4 aux = *this;
    
    elements[0][0] = aux.elements[0][0] * rhs.elements[0][0] + aux.elements[0][1] * rhs.elements[1][0] + aux.elements[0][2] * rhs.elements[2][0] + aux.elements[0][3] * rhs.elements[3][0];
    elements[0][1] = aux.elements[0][0] * rhs.elements[0][1] + aux.elements[0][1] * rhs.elements[1][1] + aux.elements[0][2] * rhs.elements[2][1] + aux.elements[0][3] * rhs.elements[3][1];
    elements[0][2] = aux.elements[0][0] * rhs.elements[0][2] + aux.elements[0][1] * rhs.elements[1][2] + aux.elements[0][2] * rhs.elements[2][2] + aux.elements[0][3] * rhs.elements[3][2];
    elements[0][3] = aux.elements[0][0] * rhs.elements[0][3] + aux.elements[0][1] * rhs.elements[1][3] + aux.elements[0][2] * rhs.elements[2][3] + aux.elements[0][3] * rhs.elements[3][3];
    
    elements[1][0] = aux.elements[1][0] * rhs.elements[0][0] + aux.elements[1][1] * rhs.elements[1][0] + aux.elements[1][2] * rhs.elements[2][0] + aux.elements[1][3] * rhs.elements[3][0];
    elements[1][1] = aux.elements[1][0] * rhs.elements[0][1] + aux.elements[1][1] * rhs.elements[1][1] + aux.elements[1][2] * rhs.elements[2][1] + aux.elements[1][3] * rhs.elements[3][1];
    elements[1][2] = aux.elements[1][0] * rhs.elements[0][2] + aux.elements[1][1] * rhs.elements[1][2] + aux.elements[1][2] * rhs.elements[2][2] + aux.elements[1][3] * rhs.elements[3][2];
    elements[1][3] = aux.elements[1][0] * rhs.elements[0][3] + aux.elements[1][1] * rhs.elements[1][3] + aux.elements[1][2] * rhs.elements[2][3] + aux.elements[1][3] * rhs.elements[3][3];
    
    elements[2][0] = aux.elements[2][0] * rhs.elements[0][0] + aux.elements[2][1] * rhs.elements[1][0] + aux.elements[2][2] * rhs.elements[2][0] + aux.elements[2][3] * rhs.elements[3][0];
    elements[2][1] = aux.elements[2][0] * rhs.elements[0][1] + aux.elements[2][1] * rhs.elements[1][1] + aux.elements[2][2] * rhs.elements[2][1] + aux.elements[2][3] * rhs.elements[3][1];
    elements[2][2] = aux.elements[2][0] * rhs.elements[0][2] + aux.elements[2][1] * rhs.elements[1][2] + aux.elements[2][2] * rhs.elements[2][2] + aux.elements[2][3] * rhs.elements[3][2];
    elements[2][3] = aux.elements[2][0] * rhs.elements[0][3] + aux.elements[2][1] * rhs.elements[1][3] + aux.elements[2][2] * rhs.elements[2][3] + aux.elements[2][3] * rhs.elements[3][3];
    
    elements[3][0] = aux.elements[3][0] * rhs.elements[0][0] + aux.elements[3][1] * rhs.elements[1][0] + aux.elements[3][2] * rhs.elements[2][0] + aux.elements[3][3] * rhs.elements[3][0];
    elements[3][1] = aux.elements[3][0] * rhs.elements[0][1] + aux.elements[3][1] * rhs.elements[1][1] + aux.elements[3][2] * rhs.elements[2][1] + aux.elements[3][3] * rhs.elements[3][1];
    elements[3][2] = aux.elements[3][0] * rhs.elements[0][2] + aux.elements[3][1] * rhs.elements[1][2] + aux.elements[3][2] * rhs.elements[2][2] + aux.elements[3][3] * rhs.elements[3][2];
    elements[3][3] = aux.elements[3][0] * rhs.elements[0][3] + aux.elements[3][1] * rhs.elements[1][3] + aux.elements[3][2] * rhs.elements[2][3] + aux.elements[3][3] * rhs.elements[3][3];
    
    return *this;
}

mat4 & mat4::transpose()
{
    float aux;
    
    aux = elements[1][0];
    elements[1][0] = elements[0][1];
    elements[0][1] = aux;
    
    aux = elements[2][0];
    elements[2][0] = elements[0][2];
    elements[0][2] = aux;
    
    aux = elements[2][1];
    elements[2][1] = elements[1][2];
    elements[1][2] = aux;
    
    aux = elements[3][0];
    elements[3][0] = elements[0][3];
    elements[0][3] = aux;
    
    aux = elements[3][1];
    elements[3][1] = elements[1][3];
    elements[1][3] = aux;
    
    aux = elements[3][2];
    elements[3][2] = elements[2][3];
    elements[2][3] = aux;
    
    return *this;
}

mat4 cgm::concat_mat4(const mat4 & lhs, const mat4 & rhs)
{
    mat4 r = lhs;
    r.concat_assign(rhs);
    
    return r;
}

mat4 cgm::transpose_mat4(const mat4 & a)
{
    mat4 b = a;
    b.transpose();
    
    return b;
}

std::ostream & cgm::print_mat4(std::ostream & os, const mat4 & rhs)
{
    int width = 8;
    
    os << " -- " << std::setw(width * 4 + 1) << std::right << "--" << std::endl;
    os << "|  " << std::setw(width) << std::left << rhs.elements[0][0] << std::setw(width) << std::left << rhs.elements[0][1] << std::setw(width) << std::left << rhs.elements[0][2] << std::setw(width) << std::left << rhs.elements[0][3] << "  |" << std::endl;
    
    
    
    
    os << "|  " << std::setw(width) << std::left << rhs.elements[1][0] << std::setw(width) << std::left << rhs.elements[1][1] << std::setw(width) << std::left << rhs.elements[1][2] << std::setw(width) << std::left << rhs.elements[1][3] << "  |" << std::endl;
    
    
    os << "|  " << std::setw(width) << std::left << rhs.elements[2][0] << std::setw(width) << std::left << rhs.elements[2][1] << std::setw(width) << std::left << rhs.elements[2][2] << std::setw(width) << std::left << rhs.elements[2][3] << "  |" << std::endl;
    os << "|_ " << std::setw(width) << std::left << rhs.elements[3][0] << std::setw(width) << std::left << rhs.elements[3][1] << std::setw(width) << std::left << rhs.elements[3][2] << std::setw(width) << std::left << rhs.elements[3][3] << " _|" << std::endl;
    
    return os;
}
