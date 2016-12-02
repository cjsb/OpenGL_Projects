#version 330 core
in vec3 v_position, v_normal;
in vec2 v_uv;

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