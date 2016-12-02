//
//  Transform.cpp
//  
//
//  Created by Mateus_Gondim on 28/06/16.
//
//

#include "Transform.hpp"
#include "vec3.hpp"
#include "vector_operations.hpp"
#include "mat4.hpp"
#include "matrix_transform.hpp"

const cgs::Transform & cgs::Transform::translate(const cgm::vec3 & translation)
{
    position = cgm::add(position, translation);
    
    return *this;
}

const cgs::Transform & cgs::Transform::set_object_to_upright(const cgm::mat4 & obj_to_upr)
{
    orientation = obj_to_upr;
    orientation_inverse = cgm::invert_orthogonal(orientation);
    
    return *this;
}

const cgs::Transform & cgs::Transform::set_position(const cgm::vec3 & pos)
{
    position = pos;
    return *this;
}

cgm::mat4 cgs::Transform::object_to_world() const
{
    cgm::mat4 model = orientation;
    model.concat_assign(cgm::translate(position));
    
    return model;
}