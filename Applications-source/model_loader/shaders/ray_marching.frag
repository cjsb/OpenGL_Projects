#version 430 core

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

//in vec3  entry_point;
in vec4  exit_point_coord;

uniform vec2 screen_size;
uniform int  voxel_grid_width;
uniform sampler2D exit_points;
uniform sampler2D entry_points;

//uniform sampler3D voxel_image;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(RGBA8) uniform image3D voxel_image;


struct Ray {
	vec3 direction;
	vec3 origin;
};


void main() 
{
	// normalize pixel coordinates
	vec3 entry_point  =  texture(entry_points, gl_FragCoord.st/screen_size).xyz;
	vec3 exit_point   =  texture(exit_points, gl_FragCoord.st/screen_size).xyz;
	

	//if(entry_point == exit_point) {
	//	discard; //background
	//}
	
	

	
	float step_size =  0.001f;///change to UNIFORM!!!!!

	vec3   ray_dir        =  exit_point - entry_point;
	float  len            =  length(ray_dir);
	vec3   delta_dir      =  normalize(ray_dir) * step_size;
	float  delta_dir_len  =  length(delta_dir);

	//////////////////////////////////////////
	vec3   dir        =  (exit_point * voxel_grid_width ) - (entry_point * voxel_grid_width);
	float  l          =  length(dir);
	vec3   d_dir      =  normalize(dir);
	float  d_dir_len  =  length(d_dir);
	/////////////////////////////////////////////
	vec3 voxel_coord = entry_point * voxel_grid_width;
	
	
	int voxel_grid_width = 1600; // make uniform
	vec4 color;
	float length_acum = 0.0f;
	for (int i = 0; i < 1000; ++i ) {
		color =  imageLoad(voxel_image, ivec3(voxel_coord) );
		
		//color  = texture(voxel_image, voxel_coord);
		if( (color.x == 0.0f) && ( color.y == 0.0f) && (color.z == 0.0f )) {
			voxel_coord += d_dir;
			length_acum += d_dir_len;
		}
		else {
		 break;
		}

		if (length_acum >= l) {
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
	}
	
	gl_FragColor = color;
    
}


/*
void main()
{
	Ray   cam_ray =  Ray(f_wld_position - cam_origin, cam_origin);
	cam_ray.direction = normalize(cam_ray.direction); 

	vec4 color;
	int i ;
	for ( i = 0; i < steps; ++i) {
		color = imageLoad(voxelImage, ivec3(gl_FragCoord.x, gl_FragCoord.y, i) );
		
		if ((color.x != 0.0f) || (color.y != 0.0f) || (color.z != 0.0f)) {
			break;
		}
	}
	if (i == steps) {
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		gl_FragColor = color;
	}
}*/