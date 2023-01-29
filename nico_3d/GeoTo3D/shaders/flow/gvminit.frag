out vec2    fColor;

void main() {

    vec4 p4;
    bool valid;
    processPixel(p4,valid);

	if (valid)
	{
		vec4 p2D = MVP*p4;
	    p2D/=p2D.w;
	    float realZ = 0.5f+0.5f*p2D.z;
		fColor = vec2(realZ,1.0f);
        gl_FragDepth = realZ;
	}
	else
		discard;
}
