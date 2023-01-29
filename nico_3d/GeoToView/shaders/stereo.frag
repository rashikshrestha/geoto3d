#version 330 core

uniform float K22;
uniform float K32;

float Zd = 0.467481;                        
float vz = 7.655192;                        
float M = -1960.37;                         
float C = 127.5;                              
uniform float Zn;                              
uniform float Zf;                              
                                                                                        
uniform sampler2DRect cTex;                    
uniform sampler2DRect dTex;                    
uniform sampler2DRect magicTex;                    
                                                                                        
out vec4 fColor;                            
                                                                                        
void main() {                    
                                                                                        
    ivec2 sz = textureSize(cTex);
	ivec2 tc = ivec2(gl_FragCoord.xy);
    if ((tc.y == sz.y-1) && (tc.x < 160))                       
    {                                                                                   
        fColor= texelFetch(magicTex,ivec2(tc.x,0));                 
        gl_FragDepth= 0.0f;                                         
    }
	else
	{
		if (tc.x<sz.x/2)                                                   
        {
            fColor		= texelFetch(cTex,tc);
            gl_FragDepth= texelFetch(dTex,tc).x;
        }                                                                               
        else                                                                            
        {                                                                               
            float depth     = texelFetch(dTex,ivec2(tc.x-sz.x/2,tc.y)).x;
            float realZ     = (K32)/((2.0f*depth-1.0f)-K22);                        
            float B         = 1/(Zf-Zn);                                                
            float tmp       = (M * (1.0f-(vz/((realZ-Zn)*B - Zd + vz))) + C)/255.0f;    
            fColor			= vec4(tmp);                                                
                                                                                        
            gl_FragDepth	= depth;                                    
        }                                                                               
    }                                                                                   
}