#version 330 core

in vec2 text_coord;
out vec4 frag_color;
uniform sampler2D sampler;

void main()
{
    frag_color = texture(sampler, text_coord);
    //frag_color =  vec4(1.0f, 0.5f, 0.2f, 1.0f);
}