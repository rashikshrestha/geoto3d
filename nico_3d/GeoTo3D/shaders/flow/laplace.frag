#version 330 core

uniform sampler2D   cTex;
uniform int         lod;
uniform float       sigma;

out vec4 fColor;

void main() {

    ivec2 tc = ivec2(gl_FragCoord.xy);
    ivec2 sz = textureSize(cTex,lod);

    int border = max(8,min(sz.x,sz.y)/100);

    vec4 p0 = texelFetch(cTex,ivec2(max(min(tc.x-1,sz.x-1),0),max(min(tc.y-1,sz.y-1),0)),lod);
    vec4 p1 = texelFetch(cTex,ivec2(max(min(tc.x  ,sz.x-1),0),max(min(tc.y-1,sz.y-1),0)),lod);
    vec4 p2 = texelFetch(cTex,ivec2(max(min(tc.x+1,sz.x-1),0),max(min(tc.y-1,sz.y-1),0)),lod);
    vec4 p3 = texelFetch(cTex,ivec2(max(min(tc.x-1,sz.x-1),0),max(min(tc.y  ,sz.y-1),0)),lod);
    vec4 p4 = texelFetch(cTex,ivec2(max(min(tc.x  ,sz.x-1),0),max(min(tc.y  ,sz.y-1),0)),lod);
    vec4 p5 = texelFetch(cTex,ivec2(max(min(tc.x+1,sz.x-1),0),max(min(tc.y  ,sz.y-1),0)),lod);
    vec4 p6 = texelFetch(cTex,ivec2(max(min(tc.x-1,sz.x-1),0),max(min(tc.y+1,sz.y-1),0)),lod);
    vec4 p7 = texelFetch(cTex,ivec2(max(min(tc.x  ,sz.x-1),0),max(min(tc.y+1,sz.y-1),0)),lod);
    vec4 p8 = texelFetch(cTex,ivec2(max(min(tc.x+1,sz.x-1),0),max(min(tc.y+1,sz.y-1),0)),lod);

    float alpha = p4.w;
    if ( (tc.x<border) || (tc.x>sz.x-1-border) || (tc.y<border) || (tc.y>sz.y-1-border) )
    	alpha = 0.0f;

#if 0

    vec4 result = vec4(0.0f);

    result-= 1.0f*p0;
    result-= 2.0f*p1;
    result-= 1.0f*p2;
    result-= 2.0f*p3;
    result+=12.0f*p4;
    result-= 2.0f*p5;
    result-= 1.0f*p6;
    result-= 2.0f*p7;
    result-= 1.0f*p8;

    result.x = 0.0f;
    result.z = 0.0f;

    result.xyz = 20.0f*sigma*sigma*result.xyz;

    fColor = vec4( result.xyz , alpha);

#else

    vec4 result  = vec4(0.0f);

	result.x 	 = p4.y;

    result.y 	+= 1.0f*abs(p0.y-p4.y);
    result.y 	+= 2.0f*abs(p1.y-p4.y);
    result.y 	+= 1.0f*abs(p2.y-p4.y);
    result.y 	+= 2.0f*abs(p3.y-p4.y);
    result.y 	+= 2.0f*abs(p5.y-p4.y);
    result.y 	+= 1.0f*abs(p6.y-p4.y);
    result.y 	+= 2.0f*abs(p7.y-p4.y);
    result.y 	+= 1.0f*abs(p8.y-p4.y);
    result.y     = result.y/12.0f;

    result.z 	-= 1.0f*p0.y;
    result.z 	-= 2.0f*p1.y;
    result.z 	-= 1.0f*p2.y;
    result.z 	-= 2.0f*p3.y;
    result.z 	+=12.0f*p4.y;
    result.z 	-= 2.0f*p5.y;
    result.z 	-= 1.0f*p6.y;
    result.z 	-= 2.0f*p7.y;
    result.z 	-= 1.0f*p8.y;
    result.z     = result.z/12.0f;

    result.w 	 = alpha;

	fColor 		 = result*vec4(1.0f,4.0*sigma,16*sigma*sigma,1.0f);

#endif

}
