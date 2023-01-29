#include <vector_functions.h>

inline   __host__ __device__ float4 operator+(float4 a, float4 b) {
    return make_float4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
}

inline   __host__ __device__ float4 operator*(float a, float4 b) {
    return make_float4(a*b.x,a*b.y,a*b.z,a*b.w);
}

struct __align__(16) cuda_m4x4 {

    __host__ __device__ float4 operator*(const float4 & v) {
        return v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3];
    }

    inline  __host__ __device__ float4 & operator[](int i) {
        return m[i];
    }

    float4 m[4];
};
