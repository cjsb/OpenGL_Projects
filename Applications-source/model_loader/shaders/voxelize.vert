#version 430 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec3 g_normal;
out vec2 g_uv;
out vec3 g_position;



 void main()
 {
	g_position  =  v_position;
	g_normal    =  v_normal;
	g_uv        =  v_uv;
	gl_Position = vec4(v_position, 1.0f);
 }