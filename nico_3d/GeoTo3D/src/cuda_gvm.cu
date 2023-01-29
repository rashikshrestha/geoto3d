#include <stdio.h>

#include "ncgl.h"

#include <cuda.h>
#include <cudaGL.h>
#include <cuda_gl_interop.h>
#include <cuda_runtime_api.h>
//#include <cutil_inline.h>
#include "cuda_gvm.h"

#define Xsize 16
#define Ysize 16

surface<void, 2> outputSurfRef;

__shared__ cudaGVM  gvm;
__shared__ int      nrsteps;
__shared__ float3   campos;
__shared__ int3     indexx[Ysize][Xsize];

struct privar {
    uint    vindexx;
    float3  dir3D;
    int		level;
    bool 	inside;
    float3 	f1;
    float3 	f2;
    float 	t;
    float3  bbox_p1;
    float3  bbox_sz;
    float3 	bbox_step;
};


inline  __device__ int3 truncf(float3 a) {
    return make_int3(truncf(a.x),truncf(a.y),truncf(a.z));
}

inline  __device__ int3 operator+(int3 a, int b) {
    return make_int3(a.x+b,a.y+b,a.z+b);
}

inline  __device__ float3 operator+(float3 a, float3 b) {
    return make_float3(a.x+b.x,a.y+b.y,a.z+b.z);
}

inline  __device__ float3 operator-(float3 a, float3 b) {
    return make_float3(a.x-b.x,a.y-b.y,a.z-b.z);
}

inline  __device__ float3 operator-(int3 a, float3 b) {
    return make_float3(a.x-b.x,a.y-b.y,a.z-b.z);
}

inline  __device__ float3 operator*(float a, float3 b) {
    return make_float3(a*b.x,a*b.y,a*b.z);
}

inline  __device__ float3 operator/(float3 a, float3 b) {
    return make_float3(a.x/b.x , a.y/b.y , a.z/b.z);
}

inline  __device__ float3 operator/(float3 a, int b) {
    return make_float3(a.x/b , a.y/b , a.z/b);
}

inline  __device__ int3 max(int3 a, int3 b) {
    return make_int3(max(a.x,b.x) , max(a.y,b.y) , max(a.z,b.z));
}

inline  __device__ int3 floor(float3 a) {
    return make_int3(floor(a.x) , floor(a.y) , floor(a.z));
}


int3 __device__ realIndex(privar & pdata) {
    int3 i_out;
    i_out.x = (pdata.dir3D.x<0.0f ? nrsteps-1-indexx[threadIdx.y][threadIdx.x].x : indexx[threadIdx.y][threadIdx.x].x);
    i_out.y = (pdata.dir3D.y<0.0f ? nrsteps-1-indexx[threadIdx.y][threadIdx.x].y : indexx[threadIdx.y][threadIdx.x].y);
    i_out.z = (pdata.dir3D.z<0.0f ? nrsteps-1-indexx[threadIdx.y][threadIdx.x].z : indexx[threadIdx.y][threadIdx.x].z);
    return i_out;
}

bool __device__ testIndex(privar & pdata) {
	return ((indexx[threadIdx.y][threadIdx.x].x>=0) && (indexx[threadIdx.y][threadIdx.x].x<nrsteps) && (indexx[threadIdx.y][threadIdx.x].y>=0) && (indexx[threadIdx.y][threadIdx.x].y<nrsteps) && (indexx[threadIdx.y][threadIdx.x].z>=0) && (indexx[threadIdx.y][threadIdx.x].z<nrsteps));
}

bool __device__ testVox(privar & pdata) {
	if (pdata.inside)
	{
	    int3 tmp = realIndex(pdata);

		pdata.vindexx   = uint(0);
		pdata.level 	  = gvm.maxlevel;
		while ((gvm.voxels[pdata.vindexx].child>0) && (pdata.level >(gvm.maxlevel-gvm.lod)))
		{
			pdata.level--;
			int offset = 0;
			offset|= ((tmp.x>>pdata.level)&0x1)<<0;
			offset|= ((tmp.y>>pdata.level)&0x1)<<1;
			offset|= ((tmp.z>>pdata.level)&0x1)<<2;
			//offset = bitfieldInsert(offset, (tmp.x>>pdata.level), 0 , 1);
			//offset = bitfieldInsert(offset, (tmp.y>>pdata.level), 1 , 1);
			//offset = bitfieldInsert(offset, (tmp.z>>pdata.level), 2 , 1);
			pdata.vindexx = gvm.voxels[pdata.vindexx].child+offset;
		}

		int mask = ~((1<<pdata.level)-1);
		indexx[threadIdx.y][threadIdx.x].x &= mask;
		indexx[threadIdx.y][threadIdx.x].y &= mask;
		indexx[threadIdx.y][threadIdx.x].z &= mask;

		return ((gvm.voxels[pdata.vindexx].carve&0xFF) == 0x00);
	}
	else
		return true;
};

void __device__ outerIntersect(privar & pdata) {

	float3  tmp = -1.0f*(pdata.f1/pdata.f2);
	pdata.t           = max(tmp.x,max(tmp.y,tmp.z));
	indexx[threadIdx.y][threadIdx.x] 	    = (int3)truncf(pdata.f1+pdata.t*pdata.f2);
}

void __device__ innerIntersect(privar & pdata) {

	int3   planes  = indexx[threadIdx.y][threadIdx.x]+(1<<pdata.level);
	float3    tmp  = (planes-pdata.f1)/pdata.f2;

            pdata.t       = min(tmp.x,min(tmp.y,tmp.z));
	int3   result 	= (int3)floor(pdata.f1+pdata.t*pdata.f2);

	if (pdata.t==tmp.x)
	{
		result.x= planes.x;
		pdata.inside  = result.x<nrsteps;
	}
	else if (pdata.t==tmp.y)
	{
		result.y= planes.y;
		pdata.inside  = result.y<nrsteps;
	}
	else
	{
		result.z= planes.z;
		pdata.inside  = result.z<nrsteps;
	}

	indexx[threadIdx.y][threadIdx.x] = max(indexx[threadIdx.y][threadIdx.x],result);

}

void __device__ processPixel(float3 & p3 , bool & valid , privar & pdata) {

    pdata.bbox_p1     = gvm.bpos;
    pdata.bbox_sz     = gvm.bsize;

    if (pdata.dir3D.x<0.0f)
    {
        pdata.bbox_p1.x+=  pdata.bbox_sz.x;
        pdata.bbox_sz.x = -pdata.bbox_sz.x;
    }
    if (pdata.dir3D.y<0.0f)
    {
        pdata.bbox_p1.y+=  pdata.bbox_sz.y;
        pdata.bbox_sz.y = -pdata.bbox_sz.y;
    }
    if (pdata.dir3D.z<0.0f)
    {
        pdata.bbox_p1.z+=  pdata.bbox_sz.z;
        pdata.bbox_sz.z = -pdata.bbox_sz.z;
    }

	pdata.bbox_step 	= pdata.bbox_sz/nrsteps;

	pdata.f1 			= (campos-pdata.bbox_p1)/pdata.bbox_step;
	pdata.f2			= pdata.dir3D/pdata.bbox_step;

	indexx[threadIdx.y][threadIdx.x] 	        = (int3)floor(pdata.f1);
	bool    test 	= true;
	valid 	        = false;

	pdata.inside = testIndex(pdata);
	if (!pdata.inside)
	{
		outerIntersect(pdata);
		pdata.inside 	= (pdata.t>0.0f) && testIndex(pdata);
		valid	= (testVox(pdata) != test);
	}
	else
	{
		test = testVox(pdata);
	}

	int it = 0;
	while ((pdata.inside) && (!valid) && (it<1000))
	{
		innerIntersect(pdata);
		valid  = (!testVox(pdata));// != test);
		it++;
	}

    p3 = campos+pdata.t*pdata.dir3D;
}





__global__ void drawGVM(unsigned int w, unsigned int h , cudaGVM gvm_) {
    privar pdata;

    int x = blockDim.x * blockIdx.x + threadIdx.x;
    int y = blockDim.y * blockIdx.y + threadIdx.y;

    if ((threadIdx.x==0) && (threadIdx.y==0))
    {
        gvm         = gvm_;
        nrsteps		= (1<<gvm.maxlevel);
        campos      = make_float3(gvm.MV[3].x,gvm.MV[3].y,gvm.MV[3].z);
    }
    __syncthreads();

    if ((x<w) && (y<h))
    {
        uchar4 fColor   = make_uchar4(0,0,0,0);
        float4 tmp      = gvm.Ki*make_float4((2.0f*x+1.0f)/w - 1.0f,(2.0f*y+1.0f)/h - 1.0f, -1.0f, 1.0f);
        tmp.x /= tmp.z;
        tmp.y /= tmp.z;
        tmp.z  = 1.0f;
        tmp.w  = 0.0f;
        tmp    = gvm.MV*tmp;
        pdata.dir3D  = make_float3(tmp.x,tmp.y,tmp.z);

        float3 p3;
        bool valid;
        processPixel(p3,valid,pdata);

        if (valid && (pdata.level==0))
        {
            int tmp = gvm.voxels[pdata.vindexx].carve;
            fColor  = make_uchar4((tmp>>24)&0xFF, (tmp>>16)&0xFF , (tmp>>8)&0xFF , 255);
            //float4 p2D = MVP*p4;
            //p2D/=p2D.w;
            //gl_FragDepth = 0.5f+0.5f*p2D.z;
        }

        surf2Dwrite(fColor, outputSurfRef, x * sizeof(uchar4), y);
    }
}

extern "C" void
runTest(cudaArray* my_array, unsigned int w, unsigned int h , cudaGVM gvm) {
    cudaBindSurfaceToArray(outputSurfRef, my_array);
    drawGVM<<<dim3((w+Xsize-1)/Xsize,(h+Ysize-1)/Ysize,1), dim3(Xsize,Ysize,1)>>>(w,h,gvm);
    //cutilCheckMsg("kernel launch failure");
}

