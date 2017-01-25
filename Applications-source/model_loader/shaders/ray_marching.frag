#version 430 core

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

in vec3  f_wld_position;

uniform vec3 cam_origin;
uniform vec3 box_min;
uniform vec3 box_max;
uniform int slice;
uniform int steps;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(RGBA8) uniform image3D voxelImage;

struct Ray {
	vec3 direction;
	vec3 origin;
};

bool intersect_box(Ray ray, vec3 box_min, vec3 box_max, out float t_near, out float t_far) 
{
	//compute intersection of ray with all six bbox planes
	vec3 inv_r = 1.0f / ray.direction;
	vec3 t_bot = inv_r * (box_min.xyz - ray.origin);
	vec3 t_top = inv_r * (box_max.xyz - ray.origin);

	// re-order intersections ro find smallest and largert on each axis
	vec3 t_min = min(t_top, t_bot);
	vec3 t_max = max (t_top, t_bot);

	// find the largest t_min and the smallest t_max
	vec2 t0 = max (t_min.xx, t_min.yz);
	t_near  = max(t0.x, t0.y);
	t0 = min (t_max.xx, t_max.yz);
	t_far = min (t0.x, t0.y);

	bool hit;
	if(t_near > t_far) {
		hit = false;
	}
	else {
		hit = true;
	}
	return hit;
}

void main() 
{
	Ray   cam_ray =  Ray(f_wld_position - cam_origin, cam_origin);
	cam_ray.direction = normalize(cam_ray.direction); 
	
	//calculate ray intersection with bounding box
	float  t_near, t_far;
	bool hit = intersect_box(cam_ray, box_min, box_max, t_near, t_far);
	if(!hit){
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
	if (t_near < 0.0f) { 
		t_near = 0.0f;
	}

	//calculate intersection points
	vec3  p_near =  cam_ray.origin + cam_ray.direction * t_near;
	vec3  p_far  =  cam_ray.origin + cam_ray.direction * t_far;

	// march along ray , accumulating color
	vec4 color;
	vec3 step = (p_near - p_far) / (steps - 1);
	vec3 p    = p_far;

	for (int i = 0; i < steps; ++i) {
		vec4 s = imageLoad(voxelImage, ivec3(p));
		color =  s.a * s + (1.0 - s.a) * color;
		p += step;
	}
	color /= steps;
	gl_FragColor = color;
  }
}

/*void main()
{
	vec4 color = imageLoad(voxelImage, ivec3(gl_FragCoord.x, gl_FragCoord.y, slice) );
	
	int max_it = 127;
	int i = 1;
	int coord = slice;
	while ( (color.x == 0.0f) && (color.y == 0.0f) && (color.z == 0.0f) ){
		++coord;
		++i;
		color = imageLoad(voxelImage, ivec3(gl_FragCoord.x, gl_FragCoord.y, coord) );

		if (i >= max_it) {
			break;
		}

		
	}
	if (i >= max_it) {
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		gl_FragColor = color;
	}
	
}*/