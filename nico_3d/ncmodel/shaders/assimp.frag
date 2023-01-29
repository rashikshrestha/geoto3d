#version 330

uniform sampler2D tex;

in  vec2    tc;
in  vec4    color;
in  vec3    pNormal;
out vec4    fColor;

uniform bool textured;

void main(void)
{
    if (textured)
	    fColor = texture(tex,tc);
    else
    	fColor = color;
}
