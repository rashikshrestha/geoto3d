uniform int             ival;
uniform float           dval;

out     vec4            fColor;

void main() {

    float d = calcDepth(dval);
    fColor = vec4(0.0f,10.0f*getErrorPass1(d),0.0f,0.0f);
}

