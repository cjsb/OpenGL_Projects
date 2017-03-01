#version 430 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec2 texCoord;
out vec3 vertexPosition;
out vec3 vertexNormal;

out vec3 normal_viewspace_g;
out vec3 normal_worldspace_g;
out vec3 vertexPosition_viewspace_g;
out vec3 vertexPosition_worldspace_g;
out vec3 eyePosition_worldspace_g;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{	
	vertexPosition  =  vec3(M * vec4(v_position, 1));
	vertexNormal    =  vec3(M * vec4(v_normal, 0));
	gl_Position     =  P * V * M * vec4(v_position,1);

	normal_viewspace_g           =  vec3(V * M * vec4(v_normal, 0));
	normal_worldspace_g          =  vec3(M * vec4(v_normal, 0));
	vertexPosition_viewspace_g   =  vec3(V * M * vec4(v_position, 1));
	vertexPosition_worldspace_g  =  vec3(M * vec4(v_position, 1));
	eyePosition_worldspace_g     =  vec3(inverse(V) * vec4(0,0,0,1));
}