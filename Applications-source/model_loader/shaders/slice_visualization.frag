#version 430 core

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

uniform int slice;
uniform int axis;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(RGBA8) uniform image3D voxel_image;

void main()
{
	//vec3 coord = vec3(gl_FragCoord.x, gl_FragCoord.y, 200);
	if (axis == 1) {
		gl_FragColor = imageLoad(voxel_image, ivec3(slice, gl_FragCoord.y, gl_FragCoord.x) );
	}
	else if (axis == 2) {
		gl_FragColor = imageLoad(voxel_image, ivec3(gl_FragCoord.y, slice, gl_FragCoord.x) );
	}
	else {
		gl_FragColor = imageLoad(voxel_image, ivec3(gl_FragCoord.x, gl_FragCoord.y, slice ) );
	}
	
	
	/*
	vec4 color;
	int i ;
	int steps = 5;
	for ( i = 0; i < steps; ++i) {
		color = imageLoad(voxel_image, ivec3(gl_FragCoord.x, gl_FragCoord.y, i) );
		
		if ((color.x != 0.0f) || (color.y != 0.0f) || (color.z != 0.0f)) {
			break;
		}
	}
	if (i == steps) {
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		gl_FragColor = color;
	}*/
}
