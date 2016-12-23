#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in  vec3  g_position[];
in  vec3  g_normal[];
in  vec2  g_uv[];

out  vec3  f_position;
out  vec3  f_normal;
out  vec2  f_uv;

flat  out  int   f_axis;
flat  out  vec4  f_aabb;

uniform  mat4  u_mvp_x;
uniform  mat4  u_mvp_y;
uniform  mat4  u_mvp_z;
uniform  int   u_voxel_grid_width;
uniform  int   u_voxel_grid_height;

void main()
{
	
	vec3   triangle_normal =  normalize(cross(g_position[1]-g_position[0], g_position[2]-g_position[0]) );
	float  n_dot_x_axis    =  abs(triangle_normal.x);
	float  n_dot_y_axis    =  abs(triangle_normal.y);
	float  n_dot_z_axis    =  abs(triangle_normal.z);

	mat4 ortho_proj;
	
	//find the dominant axis, i.e, the axis that maximizes the projected area 
	if (n_dot_x_axis > n_dot_y_axis && n_dot_x_axis > n_dot_z_axis) {
		ortho_proj  =  u_mvp_x;
		f_axis      =  1;
	}
	else if (n_dot_y_axis > n_dot_x_axis && n_dot_y_axis > n_dot_z_axis) {
		ortho_proj  =  u_mvp_y;
		f_axis      =  2;
	}
	else {
		ortho_proj  =  u_mvp_z;
		f_axis      =  3;
	}
	
	//project the triangle orthographically along the dominant axis
	vec4 position[3];
	position[0]  =  ortho_proj * gl_in[0].gl_Position;
	position[1]  =  ortho_proj * gl_in[1].gl_Position;
	position[2]  =  ortho_proj * gl_in[2].gl_Position;

	//enlarge the triangle for conservative rasterization
	vec4 aabb;
	vec2 h_px  =  vec2(1.0f/u_voxel_grid_width, 1.0f/u_voxel_grid_height);
	float pl   =  1.4142135637309 / u_voxel_grid_width;

	//triangle aabb
	aabb.xy = position[0].xy;
	aabb.zw = position[0].xy;

	aabb.xy = min( position[1].xy, aabb.xy );
	aabb.zw = max( position[1].xy, aabb.zw );
	
	aabb.xy = min( position[2].xy, aabb.xy );
	aabb.zw = max( position[2].xy, aabb.zw );

	//enlarge 
	aabb.xy -= h_px;
	aabb.zw += h_px;

	f_aabb = aabb;

	//3 triangle edge plane
	vec3 e0 = vec3( position[1].xy - position[0].xy, 0 );
	vec3 e1 = vec3( position[2].xy - position[1].xy, 0 );
	vec3 e2 = vec3( position[0].xy - position[2].xy, 0 );
	vec3 n0 = cross( e0, vec3(0,0,1) );
	vec3 n1 = cross( e1, vec3(0,0,1) );
	vec3 n2 = cross( e2, vec3(0,0,1) );

	//dilate the triangle
	position[0].xy = position[0].xy + pl*( (e2.xy/dot(e2.xy,n0.xy)) + (e0.xy/dot(e0.xy,n2.xy)) );
	position[1].xy = position[1].xy + pl*( (e0.xy/dot(e0.xy,n1.xy)) + (e1.xy/dot(e1.xy,n0.xy)) );
	position[2].xy = position[2].xy + pl*( (e1.xy/dot(e1.xy,n2.xy)) + (e2.xy/dot(e2.xy,n1.xy)) );


	gl_Position  =  position[0];
	f_position   =  position[0].xyz;
	f_normal     =  g_normal[0];
	f_uv         =  g_uv[0];
	EmitVertex();

	gl_Position  =  position[1];
	f_position   =  position[1].xyz;
	f_normal     =  g_normal[1];
	f_uv         =  g_uv[1];
	EmitVertex();

	gl_Position  =  position[2];
	f_position   =  position[2].xyz;
	f_normal     =  g_normal[2];
	f_uv         =  g_uv[2];
	EmitVertex();

	EndPrimitive();
}