#version 430 core

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

uniform int slice;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(RGBA8) uniform image3D voxelImage;

void main()
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
	
}