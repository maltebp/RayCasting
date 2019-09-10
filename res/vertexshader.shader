#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 vColor;
out vec4 fColor;

void main(){
    gl_Position = position;
    fColor = vColor;
}