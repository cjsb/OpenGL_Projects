#version 430


in vec3 normal_viewspace_f;
in vec3 normal_worldspace_f;
in vec3 vertexPosition_viewspace_f;
in vec3 vertexPosition_worldspace_f;
in vec3 eyePosition_worldspace_f;

layout (location = 0) out vec4 gl_FragColor;

layout(RGBA8) uniform image3D voxelImage;


layout (binding = 0, offset = 0) uniform atomic_uint u_voxel_frag_count;

uniform layout(binding = 0, rgb10_a2ui)  uimageBuffer  u_voxel_pos;
uniform layout(binding = 1, rgba8 )      imageBuffer   u_voxel_col;

uniform float  sceneScale;
uniform int    u_pass_no;
uniform int    u_mode;

void main() 
{
	vec3 color = vec3(0.0f, 1.0f, 1.0f);

	ivec3 size = imageSize(voxelImage);
	ivec3 texCoord = ivec3((vertexPosition_worldspace_f / sceneScale  + vec3(1)) / 2 * size);
    
	if (u_mode == 1) { // using texture to store the voxelized scene
		imageStore(voxelImage, ivec3(texCoord), vec4(color, 1.0f));
	}
	else { // using a sparse voxel octree to voxelize the scene
		uvec4 coord = uvec4(texCoord, 0);
		uint idx = atomicCounterIncrement(u_voxel_frag_count);
		if (u_pass_no == 2) {
			imageStore(u_voxel_pos, int(idx), coord); // voxel position in the 3d voxel grid
			imageStore(u_voxel_col, int(idx), vec4(color, 1.0f)); // voxel color attribute
		}
	}
	
	
	

    gl_FragColor = vec4(color, 1.0f);
}