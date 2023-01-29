layout(location = 0) in ivec4 mPosition;

out float   edge;

void main(void)
{
    vPBO[gl_VertexID]   = 0;
    vec3 p3D            = 0.001f*(mPosition.xyz);
    gl_Position         = vec4( p3D ,1.0f);
    edge                = 0.001f*(1<<mPosition.w);
}
