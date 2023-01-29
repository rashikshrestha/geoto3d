#version 400 core
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

uniform mat4 MVP;
uniform vec3 campos;
uniform int  maxlevel;
uniform int  lod;

in vec3 p3D;
vec3    dir3D;

vec3 	bbox_step;
int		nrsteps;
int		level;
bool 	inside;
vec3    bbox_p1;
vec3    bbox_sz;

vec3 	f1;
vec3 	f2;

uint    vindex;
ivec3   index;
float 	t;

//unsigned int cache[16];
//ivec3        last = ivec3(0,0,0);

uniform int*  child;
uniform int*  rgbm;
uniform int*  color;

ivec3 realIndex() {
    ivec3 i_out;
    i_out.x = (dir3D.x<0.0f ? nrsteps-1-index.x : index.x);
    i_out.y = (dir3D.y<0.0f ? nrsteps-1-index.y : index.y);
    i_out.z = (dir3D.z<0.0f ? nrsteps-1-index.z : index.z);
    return i_out;
}

bool testIndex() {
	return ((index.x>=0) && (index.x<nrsteps) && (index.y>=0) && (index.y<nrsteps) && (index.z>=0) && (index.z<nrsteps));
}

bool testVox() {
	if (inside)
	{
	    ivec3 tmp = realIndex();

		vindex    = uint(0);
		level 	  = maxlevel;
		while ((child[vindex]>0) && (level >lod))
		{
			level--;
			int offset = 0;
			offset = bitfieldInsert(offset, (tmp.x>>level), 0 , 1);
			offset = bitfieldInsert(offset, (tmp.y>>level), 1 , 1);
			offset = bitfieldInsert(offset, (tmp.z>>level), 2 , 1);
			vindex = child[vindex]+offset;
		}

		int mask = ~((1<<level)-1);
		index.x &= mask;
		index.y &= mask;
		index.z &= mask;

		return ((rgbm[vindex]&0xFF) == 0x00);
	}
	else
		return true;
};

void outerIntersect() {

	vec3    tmp = -f1/f2;
	t           = max(tmp.x,max(tmp.y,tmp.z));
	index 	    = (ivec3)trunc(f1+t*f2);
}

void innerIntersect() {

	ivec3   planes  = index+(1<<level);
	vec3    tmp     = (planes-f1)/f2;

            t       = min(tmp.x,min(tmp.y,tmp.z));
	ivec3   result 	= (ivec3)floor(f1+t*f2);

	if (t==tmp.x)
	{
		result.x= planes.x;
		inside  = result.x<nrsteps;
	}
	else if (t==tmp.y)
	{
		result.y= planes.y;
		inside  = result.y<nrsteps;
	}
	else
	{
		result.z= planes.z;
		inside  = result.z<nrsteps;
	}

	index = max(index,result);

}

void processPixel(out vec4 p4 , out bool valid) {

    dir3D = normalize(p3D - campos);

    bbox_p1     = vec3(0.0f);
    bbox_sz     = vec3(0.001f*(1<<maxlevel));

    if (dir3D.x<0.0f)
    {
        bbox_p1.x =  bbox_sz.x;
        bbox_sz.x = -bbox_sz.x;
    }
    if (dir3D.y<0.0f)
    {
        bbox_p1.y =  bbox_sz.y;
        bbox_sz.y = -bbox_sz.y;
    }
    if (dir3D.z<0.0f)
    {
        bbox_p1.z =  bbox_sz.z;
        bbox_sz.z = -bbox_sz.z;
    }

	nrsteps		= (1<<maxlevel);
	bbox_step 	= bbox_sz/nrsteps;

	f1 			= (campos-bbox_p1)/bbox_step;
	f2			= dir3D/bbox_step;

	index 	        = (ivec3)floor(f1);
	bool    test 	= true;
	valid 	        = false;

	inside = testIndex();
	if (!inside)
	{
		outerIntersect();
		inside 	= (t>0.0f) && testIndex();
		valid	= (testVox() != test);
	}
	else
	{
		test = testVox();
	}

	int it = 0;
	while ((inside) && (!valid))
	{
		innerIntersect();
		valid  = (testVox() != test);
		it++;
	}

    p4 = vec4(campos+t*dir3D,1.0f);
}

