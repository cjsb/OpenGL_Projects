#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec3 f_normal;
out vec2 f_uv;
out vec3 f_position;

uniform mat4 model, view, projection;

 void main()
 {
	gl_Position =  projection * view * model * vec4(v_position, 1.0f);
	f_uv        =  v_uv;
	f_position  =  vec3(model * vec4(v_position, 1.0f));
	f_normal    =  v_normal; 
 }