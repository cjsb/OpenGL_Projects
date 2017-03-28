#version 430 core

flat in int   f_axis;
flat in vec4  f_aabb;

in  vec3  f_position;
in  vec3  f_normal;
in  vec2  f_uv;

layout (location = 0) out vec4 gl_FragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

//atomic counter
layout ( binding = 0, offset = 0 ) uniform atomic_uint u_voxelFragCount;

uniform layout(binding = 0, rgb10_a2ui) uimageBuffer u_voxelPos;
uniform layout(binding = 1, rgba8 ) imageBuffer u_voxelKd;

//layout(RGBA8) uniform image3D voxelImage;

uniform  int   u_voxel_grid_width;
uniform  int   u_voxel_grid_height;
uniform  int u_bStore;

void main()
{
    if( f_position.x < f_aabb.x || f_position.y < f_aabb.y || f_position.x > f_aabb.z || f_position.y > f_aabb.w )
	   discard ;

    vec4 data = vec4(1.0,0.0,0.0,0.0);
    //ivec3 temp = ivec3( gl_FragCoord.x, gl_FragCoord.y, u_width * gl_FragCoord.z ) ;
	uvec4 temp = uvec4( gl_FragCoord.x, gl_FragCoord.y, u_voxel_grid_width * gl_FragCoord.z, 0 ) ;
	uvec4 texcoord;
	if( f_axis == 1 )
	{
	    texcoord.x = u_voxel_grid_width - temp.z;
		texcoord.z = temp.x;
		texcoord.y = temp.y;
	}
	else if( f_axis == 2 )
    {
	    texcoord.z = temp.y;
		texcoord.y = u_voxel_grid_width-temp.z;
		texcoord.x = temp.x;
	}
	else
	    texcoord = temp;

	uint idx = atomicCounterIncrement( u_voxelFragCount );
	if( u_bStore == 1 ) {
	   imageStore( u_voxelPos, int(idx), texcoord );
	   imageStore( u_voxelKd, int(idx), vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
	}

	//imageStore( u_voxelImage, texcoord, data );
	//gl_FragColor = vec4( 1, 1, 1, 1 );
}













/*
void main()
{
	if (f_position.x < f_aabb.x || f_position.y < f_aabb.y || f_position.x > f_aabb.z || f_position.y > f_aabb.w) {
	   discard ;
	}

	uvec4 temp = uvec4( gl_FragCoord.x, gl_FragCoord.y, u_voxel_grid_width * gl_FragCoord.z, 0 ) ;
	uvec4 tex_coord;
	if( f_axis == 1 )
	{
	    tex_coord.x = u_voxel_grid_width - temp.z;
		tex_coord.z = temp.x;
		tex_coord.y = temp.y;
	}
	else if( f_axis == 2 )
    {
	    tex_coord.z = temp.y;
		tex_coord.y = u_voxel_grid_width-temp.z;
		tex_coord.x = temp.x;
	}
	else {
	    tex_coord = temp;
	}
	imageStore( voxelImage, ivec3(tex_coord), vec4(0.0f, 1.0f, 1.0f, 1.0f) );
	gl_FragColor = vec4( 1, 1, 1, 1 );
}*/