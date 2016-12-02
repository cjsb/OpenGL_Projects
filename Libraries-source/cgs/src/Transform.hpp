//
//  Transform.hpp
//  
//
//  Created by Mateus_Gondim on 28/06/16.
//
//

#ifndef _TRANSFORM_HPP

#define _TRANSFORM_HPP
#include "vec3.hpp"
#include "mat4.hpp"
#include "matrix_transform.hpp"

namespace cgs {
    class Transform {
    public:
        Transform() = default;
        Transform(const cgm::vec3 & initial_pos, const cgm::mat4 & obj_to_upr, const cgm::vec3 & initial_scale):
                 position(initial_pos), scale(initial_scale), orientation(obj_to_upr)
        {
            orientation_inverse = cgm::invert_orthogonal(orientation);
        }
    
        const cgm::mat4 & object_to_upright() const {return orientation;}
        const cgm::mat4 & upright_to_object() const {return orientation_inverse;}
              cgm::mat4   object_to_world()   const;
        const cgm::vec3 & get_position()      const {return position;}
        
        const Transform & set_object_to_upright(const cgm::mat4 & obj_to_upr);
        const Transform & translate(const cgm::vec3 & translation);
        const Transform & set_position(const cgm::vec3 & pos);
    private:
        cgm::vec3  position;
        cgm::mat4  orientation; // object to upright
        cgm::mat4  orientation_inverse;
        cgm::vec3  scale = cgm::vec3(1.0f, 1.0f, 1.0f);
        
    };
}

#endif