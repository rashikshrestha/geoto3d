#version 330 core

layout(location = 0)    in		vec4 mPosition;

void main() {
    gl_Position = mPosition;
}
