#include <stdio.h>

#include "ncgl.h"

#include <cuda.h>
#include <cudaGL.h>
#include <cuda_gl_interop.h>
#include <cuda_runtime_api.h>
//#include <cutil_inline.h>
#include "cuda_bp.h"

surface<void, 2> outputSurfRef;

int getIndex(int z, int x, int y , int w, int h, int d)
{
    return (y*w+x)*d+z;
}

extern "C" void
normalizeCPU(float* dst , float* src , int w, int h, int d)
{

    //!normalize
    for (unsigned int y=0; y<h; ++y)
        for (unsigned int x=0; x<w; ++x)
        {
            float sum = 0.0f;
            for (unsigned int z=0; z<d; ++z)
                sum +=src[getIndex(z,x,y,w,h,d)];

            for (unsigned int z=0; z<d; ++z)
                dst[getIndex(z,x,y,w,h,d)] = src[getIndex(z,x,y,w,h,d)]/sum;
        }
}

extern "C" void
smoothXYCPU(float* dst , float* src , int w, int h, int d)
{

    for (unsigned int y=0; y<h; ++y)
        for (unsigned int x=0; x<w; ++x)
            for (unsigned int z=0; z<d; ++z)
            {
                float   value   = src[getIndex(z,x,y,w,h,d)];
                int     weight  = 1.0f;

                if (x>0)
                {
                    value+=src[getIndex(z,x-1,y,w,h,d)];
                    weight+=1.0f;
                }
                if (x<w-1)
                {
                    value+=src[getIndex(z,x+1,y,w,h,d)];
                    weight+=1.0f;
                }
                if (y>0)
                {
                    value+=src[getIndex(z,x,y-1,w,h,d)];
                    weight+=1.0f;
                }
                if (y<h-1)
                {
                    value+=src[getIndex(z,x,y+1,w,h,d)];
                    weight+=1.0f;
                }
                dst[getIndex(z,x,y,w,h,d)] = value/weight;
            }
}

extern "C" void
smoothZCPU(float* dst , float* src , int w, int h, int d)
{

    //!smoothZ
    for (unsigned int y=0; y<h; ++y)
        for (unsigned int x=0; x<w; ++x)
            for (unsigned int z=0; z<d; ++z)
            {
                float   value   = src[getIndex(z,x,y,w,h,d)];
                int     weight  = 1.0f;

                if (z>0)
                {
                    value+=src[getIndex(z-1,x,y,w,h,d)];
                    weight+=1.0f;
                }
                if (z<d-1)
                {
                    value+=src[getIndex(z+1,x,y,w,h,d)];
                    weight+=1.0f;
                }

                dst[getIndex(z,x,y,w,h,d)] = value/weight;
            }
}

extern "C" void
compareCPU(float* dst , float* src , int w, int h, int d)
{
    for (int z = 0; z<d; ++z)
        for (int y = 0; y<h; ++y)
            for (int x = 0; x<w; ++x)
                if (fabs(dst[(y*w+x)*d+z]-src[(y*w+x)*d+z])>0.000001)
                    fprintf(stderr,"error at (%d,%d,%d): %f %f\n",x,y,z,dst[(y*w+x)*d+z],src[(y*w+x)*d+z]);
}

__global__ void normalizeKernel(float* src_dst , float* src , int w, int h, int d, float orig_weight)
{

    int x 		= blockIdx.x;
    int y 		= blockIdx.y;
	int tid		= threadIdx.x;
	int offset 	= (y*w+x)*d;

    __shared__ float idata[1024];
    __shared__ float odata[1024];

	float value = src_dst[offset+tid];
	odata[tid] 	= idata[tid] = value*value;
	__syncthreads();

	int cnt = d;
	int hlf = (cnt+1)>>1;

	while (cnt>1) {
		if ((tid<hlf) && (hlf+tid<cnt))
			odata[tid] += odata[hlf+tid];
		__syncthreads();
		cnt = hlf;
		hlf = (cnt+1)>>1;
	}

	float tmp = d*idata[tid]/odata[0];
    if (src)
		tmp = (1.0f-orig_weight)*tmp + orig_weight*src[offset+tid]*src[offset+tid];
	src_dst[offset+tid] = tmp;

}

__global__ void smoothXYKernel(float* dst , float* src , int w, int h, int d, int range)
{

    int x 		= blockIdx.x;
    int y 		= blockIdx.y;
	int tid		= threadIdx.x;
	int offset 	= (y*w+x)*d;

    __shared__ float odata[1024];

    odata[tid] = 0.0f;

    float   tmp = 3.14159265359f/(range+1);
    float 	weight;
    float 	weight_total = 0.0f;

    int ox_min = -min(range , x		);
    int ox_max = +min(range , w-1-x	);
    int oy_min = -min(range , y		);
    int oy_max = +min(range , h-1-y	);

    for (int oy=oy_min; oy<=oy_max; ++oy)
        for (int ox=ox_min; ox<=ox_max; ++ox)
        {
	        weight		 = (0.5f + 0.5f*cos(tmp*ox))*(0.5f + 0.5f*cos(tmp*oy));
            odata[tid]	+= weight*src[((y+oy)*w+(x+ox))*d+tid];
            weight_total+= weight;
		}
	dst[offset+tid] = odata[tid]/weight_total;

}

#if 1

__global__ void smoothZKernel(float* dst , float* src , int w, int h, int d , int range)
{

    int x 		= blockIdx.x;
    int y 		= blockIdx.y;
    int tid		= threadIdx.x;
	int offset 	= (y*w+x)*d;

    float   tmp = 3.14159265359f/(range+1);
    float 	weight;
    //float 	total_weight = 0.0f;

    __shared__ float idata[1024];
    __shared__ float odata[1024];

    idata[tid] = src[offset + tid];
    odata[tid] = src[offset + tid];
    __syncthreads();

    for (int r=1; r<=range; ++r)
    {
	    weight		= 0.5f + 0.5f*cos(tmp*r);
       	odata[tid]  = max(odata[tid],weight*idata[max(tid-r,0  )]);
	    __syncthreads();
       	odata[tid]  = max(odata[tid],weight*idata[min(tid+r,d-1)]);
    	__syncthreads();
	}

	dst[offset + tid] = odata[tid];

}

#else

__global__ void smoothZKernel(float* dst , float* src , int w, int h, int d , int range)
{

    int x 		= blockIdx.x;
    int y 		= blockIdx.y;
    int tid		= threadIdx.x;
	int offset 	= (y*w+x)*d;

    float   tmp = 3.14159265359f/(range+1);
    float 	weight;
    float 	total_weight = 1.0f;

    __shared__ float idata[1024];
    __shared__ float odata[1024];

    idata[tid] = src[offset + tid];
    odata[tid] = src[offset + tid];
    __syncthreads();

    for (int r=1; r<=range; ++r)
    {
	    weight		 = 0.5f + 0.5f*cos(tmp*r);
       	odata[tid] 	+= weight*idata[max(tid-r,0  )];
		total_weight+= weight;
	    __syncthreads();
       	odata[tid] 	+= weight*idata[min(tid+r,d-1)];
		total_weight+= weight;
    	__syncthreads();
	}
   	odata[tid] /= total_weight;
   	__syncthreads();

	dst[offset + tid] = odata[tid];

}

#endif

__global__ void extractKernel(float* src , int w, int h, int d, float* zlut_dev)
{
    int x 		= blockIdx.x;
    int y 		= blockIdx.y;
	int tid		= threadIdx.x;
	int offset 	= (y*w+x)*d;

    __shared__ int 		idata[1024];
    __shared__ float 	fdata[1024];

	idata[tid] 	= tid;
	fdata[tid]	= src[offset+tid];
	__syncthreads();

	int cnt = d;
	int hlf = (cnt+1)>>1;

	while (cnt>1) {
		if ((tid<hlf) && (hlf+tid<cnt))
			if (fdata[tid]<fdata[hlf+tid])
			{
				fdata[tid] = fdata[hlf+tid];
				idata[tid] = idata[hlf+tid];
			}
		__syncthreads();
		cnt = hlf;
		hlf = (cnt+1)>>1;
	}

	if (tid == 0)
    {
#if 0
            //! interpolate
            // http://stackoverflow.com/questions/717762/how-to-calculate-the-vertex-of-a-parabola-given-three-points

            float x1 = bdepth[tid]-1;
            float x2 = bdepth[tid]  ;
            float x3 = bdepth[tid]+1;

            float y1 = src[(y*w+x)*d + max(  0, bdepth[tid]-1) ];
            float y2 = src[(y*w+x)*d +          bdepth[tid]    ];
            float y3 = src[(y*w+x)*d + min(d-1, bdepth[tid]+1) ];

            float center;
            if ((y1==y2) && (y1==y3))
                center = x2;
            else
            {
                float B         = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3));
                float A         = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2));
                center          = -0.5f*B/A;
            }

            float2 fColor  = make_float2((center+0.5f)/d , 1.0f);
            surf2Dwrite(fColor, outputSurfRef, x * sizeof(float2), y);
#else

	float resdepth = zlut_dev[idata[0]];
	float2 fColor  = make_float2(resdepth , 1.0f);
    surf2Dwrite(fColor, outputSurfRef, x * sizeof(float2), y);
#endif
	}
}

int my_it = 0;

extern "C" void
save(float* src , int w, int h, int d) {

	if (my_it == 5) {

		int 	nrpix2d = w*h;
		int 	nrpix3d = nrpix2d*d;
		int 	nrdat3d	= nrpix3d*sizeof(float);
		float* 	tmp 	= (float*)malloc(nrdat3d);

		cudaMemcpy(tmp,src,nrdat3d,cudaMemcpyDeviceToHost);

		float 	maxval 	= 0.0f;
		for (int i=0;i<nrpix3d;++i)
			maxval = std::max(maxval,tmp[i]);

		fprintf(stderr,"maxval = %f\n",maxval);

		float fact = 255.0f;///maxval;

		ncImageBuffer<unsigned char> imbuf(w,h);
		for (int z=0;z<d;++z)
		{
			float* slice = &tmp[z];
			for (int p=0;p<nrpix2d;++p)
			{
				imbuf[p] = (int)(fact*(*slice));
				slice+=d;
			}

			char imname[1024];
			sprintf(imname,"/tmp/%04d_%04d.png",my_it,z);
			ncc::writeImage(imname,imbuf);
		}

		free(tmp);
	}

    my_it++;
}

extern "C" void
beliefPropagation(cudaArray* my_array, float* idata0 , float* idata1 , float* idata2 , int w, int h, int d , float* zlut_dev)
{

    int nrit        = 4;
    int smoothZ     = 32;
    int smoothXY    = 1;

    float init_orig_weight = 0.0f;
    float exit_orig_weight = 0.25f;

    cudaBindSurfaceToArray(outputSurfRef, my_array);
	//save(idata0 , w, h, d);
    normalizeKernel<<<dim3(w,h,1) , dim3(d,1,1)>>>(idata0 , 0 , w, h, d, 0.0f);
	cudaMemcpy(idata1,idata0,w*h*d*sizeof(float),cudaMemcpyDeviceToDevice);

    for (unsigned int it=0; it<nrit; ++it)
    {
        smoothZKernel 	<<<dim3(w,h,1) , dim3(d,1,1)>>>(idata2 , idata0 , w, h, d, smoothZ);
        smoothXYKernel	<<<dim3(w,h,1) , dim3(d,1,1)>>>(idata0 , idata2 , w, h, d, smoothXY);

        float lrp = (float(it)/float(nrit-1));
        normalizeKernel	<<<dim3(w,h,1) , dim3(d,1,1)>>>(idata0 , idata1 , w, h, d, (1.0f-lrp)*init_orig_weight + lrp*exit_orig_weight);
    }

    extractKernel<<<dim3(w,h,1) , dim3(d,1,1)>>>(idata0 , w, h, d, zlut_dev);
    //cutilCheckMsg("kernel launch failure");
}

