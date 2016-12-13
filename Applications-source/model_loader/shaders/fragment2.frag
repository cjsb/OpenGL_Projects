#version 330 core
in vec3 f_position;
in vec3 f_normal;
in vec2 f_uv;

out vec4 f_color;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

void main()
{
	vec3 light_ambient   =  light_color * 0.2f;
	vec3 light_diffuse   =  light_color * 0.5f;
	vec3 light_specular  =  light_color * 1.0f; 
	float shininess           =  51.2f;
	
	//ambient
	vec3 mat_ambient = vec3(0.19225f, 0.19225f, 0.19225f);
	vec3 ambient = light_ambient * mat_ambient;
	
	//diffuse
	vec3   mat_diffuse =  vec3(0.50754f, 0.50754f, 0.50754f);
	vec3   normal      =  normalize(f_normal);
	vec3   light_dir   =  normalize(light_pos - f_position);
	float  diff        =  max(dot(normal, light_dir), 0.0f);
	vec3   diffuse     =  light_diffuse * diff *  mat_diffuse;
	
	//specular
	vec3   mat_specular = vec3(0.508273f, 0.508273f, 0.508273f);
	vec3   view_dir     =  normalize(cam_pos - f_position);
	vec3   reflect_dir  =  reflect(-light_dir, normal);
	float  spec         =  pow(max(dot(view_dir, reflect_dir), 0.0f), shininess);
	vec3   specular     =   light_specular * spec * mat_specular;
	  
	f_color = vec4(ambient + diffuse + specular, 1.0f);
}