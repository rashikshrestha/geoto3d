layout(location = 0) out vec4    fColor;

void main(void) {

    vec4 p4;
    bool valid;
    processPixel(p4,valid);

	if (valid)
	{
		int     tmp     = rgbm[vindex];
        ivec4   fcol    = ivec4((tmp>>24)&0xFF, (tmp>>16)&0xFF , (tmp>>8)&0xFF , 255);
        fColor = vec4(fcol/255.0f);
        //fColor = vec4(level*20.0f/255.0f);

	    vec4 p2D = MVP*p4;
	    p2D/=p2D.w;
		gl_FragDepth = 0.5f+0.5f*p2D.z;
	}
	else
		discard;
}
