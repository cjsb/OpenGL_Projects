//
//  Camera.hpp
//  
//
//  Created by Mateus_Gondim on 27/06/16.
//
//

#ifndef _CAMERA_HPP

#define _CAMERA_HPP
#include "mat4.hpp" 
#include "Transform.hpp"
namespace cgs {
    class Camera {
    public:
        enum   Projection_type {PERSPECTIVE, ORTHOGRAPHIC};
                Camera();
        void    scale_film_gate(int new_image_width, int new_image_heigt);
        void    set_mode(Projection_type m);
        const   cgm::mat4 & get_projection() const {return projection;}
        Transform & get_transform() {return transform;}
		const Transform & get_transform() const { return transform; }
        int     get_image_width()  const {return image_width;}
        int     get_image_height() const {return image_height;}
        float   get_device_aspect_ratio() const { return image_width / (float)image_height ;}
        void    get_bnd(float & l, float & r, float & b, float & t, float & n, float & f) const;
		void    set_far_clipping_plane(const float f);
    private:
        void   compute_fov();
        void   compute_bnd();
        void   compute_proj_mat();
        float  film_width = 0.980f, film_height = 0.735f; // in inches
        float  film_aspect_ratio;
        float  focal_length = 35; // in mm
        float  fov_h;
        float  fov_v;
        int    image_width = 640, image_height = 480; // in px
        float  device_aspect_ratio;
        
        float  near = 0.1f;
        float  far  = 100.0f;
        float  left, right, bottom, top;
        
        cgm::mat4        projection;
        Transform        transform;
        Projection_type  mode = PERSPECTIVE;
    };
}

#endif