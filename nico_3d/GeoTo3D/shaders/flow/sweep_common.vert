#version 330 core

layout(location = 0)    in		vec4 mPosition;

out vec4 mPos;

void main() {

    gl_Position = vec4(mPosition.xy,0.0f,1.0f);
    mPos        = vec4(mPosition.zw,0.0f,1.0f);
}
