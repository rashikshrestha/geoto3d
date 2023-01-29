#version 330 core
in float zpos;
in vec4  color;

out vec4    fColor;
void main() {

    fColor          = color;
    gl_FragDepth    = zpos;
}
