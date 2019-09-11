#version 410 core

uniform vec2 center;
uniform float radius;

out vec4 color;
in vec4 fColor;
in vec2 TexCoord;

void main(){


    if( distance(TexCoord, center) >  radius ) discard;

    color = fColor;    
}