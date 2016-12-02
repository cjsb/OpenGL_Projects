#version 410 core

in     vec3 vertex_position;
out    vec3 frag_wld_coord;

uniform mat4 model, view, projection;


void main()
{
    frag_wld_coord = (model * vec4(vertex_position, 1.0f)).xyz;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}