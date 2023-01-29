layout(points) in;
layout(triangle_Strip, max_vertices = 14) out;

in  float   edge[1];

flat out int  gIndex;

bool insidevp(vec4 point) {
    return ((point.w>0.0f) && (abs(point.x) <= point.w) && (abs(point.y) <= point.w) && (abs(point.z) <= point.w));
}

void main()
{
    vec4 p0 = MVP*gl_in[0].gl_Position;
    vec4 p1 = p0 + edge[0]*(MVP[0]              );
    vec4 p2 = p0 + edge[0]*(       MVP[1]       );
    vec4 p3 = p0 + edge[0]*(MVP[0]+MVP[1]       );
    vec4 p4 = p0 + edge[0]*(              MVP[2]);
    vec4 p5 = p0 + edge[0]*(MVP[0]       +MVP[2]);
    vec4 p6 = p0 + edge[0]*(       MVP[1]+MVP[2]);
    vec4 p7 = p0 + edge[0]*(MVP[0]+MVP[1]+MVP[2]);

    gIndex  = gl_PrimitiveIDIn;

    float halfedge = 0.5f*edge[0];

    vec3 vcenter = gl_in[0].gl_Position.xyz + halfedge;
    vec3 diff    = abs(vcenter-campos);
    if ((diff.x<=halfedge) && (diff.y<=halfedge) && (diff.z<=halfedge))
    {
        //! campos inside voxel
        vPBO[gIndex]   = 8;
    }
    else
    {
        //! voxel at edge of viewport
        bool edgetest = insidevp(p0) && insidevp(p1) && insidevp(p2) && insidevp(p3) && insidevp(p4) && insidevp(p5) && insidevp(p6) && insidevp(p7);

        vPBO[gIndex]   = (  edgetest ? 0 : 4);

        gl_Position = p3;
        EmitVertex();
        gl_Position = p2;
        EmitVertex();
        gl_Position = p7;
        EmitVertex();
        gl_Position = p6;
        EmitVertex();
        gl_Position = p4;
        EmitVertex();
        gl_Position = p2;
        EmitVertex();
        gl_Position = p0;
        EmitVertex();
        gl_Position = p3;
        EmitVertex();
        gl_Position = p1;
        EmitVertex();
        gl_Position = p7;
        EmitVertex();
        gl_Position = p5;
        EmitVertex();
        gl_Position = p4;
        EmitVertex();
        gl_Position = p1;
        EmitVertex();
        gl_Position = p0;
        EmitVertex();

        EndPrimitive();
    }
}
