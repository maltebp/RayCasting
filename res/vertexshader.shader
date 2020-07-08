#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 vColor;
layout(location=0) in vec2 aTexCoord;

out vec4 fColor;
out vec2 TexCoord;


void main(){
    gl_Position = ((position-vec4(680,480,0,0))*vec4(1,-1,1,1))  / vec4(680,480,1,1);
    fColor = vColor;
    TexCoord = aTexCoord; 
}   