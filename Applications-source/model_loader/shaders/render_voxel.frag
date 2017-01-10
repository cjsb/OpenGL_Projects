#version 430 core

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(RGBA8) uniform image3D voxelImage;

void main()
{
	vec4 color = imageLoad(voxelImage, ivec3(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z * 128) );
	
	if ( (color.x == 0.0f) && (color.y == 0.0f) && (color.z == 0.0f) ){
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		gl_FragColor = color;
	}
	
}