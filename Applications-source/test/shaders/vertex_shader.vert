#version 330 core 

in vec3 vert_pos;
in vec2 vert_text_coord;

out vec2 text_coord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    text_coord  =  vert_text_coord;
    text_coord.y = 1.0f - text_coord.y;
    
    gl_Position =  projection * view * vec4(vert_pos, 1.0f);
}