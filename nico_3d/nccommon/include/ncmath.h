#ifndef __NC_MATH__
#define __NC_MATH__

#undef min
#undef max
//#undef round

#ifdef WIN32
#include <limits.h>
#endif

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include <iostream>
#include <stdio.h>
#include <limits>
#include <cmath>

namespace ncc
{


template <unsigned int N, class T>
class nc_vec
{
    T data[N];
};


template <class T>
struct nc_vec <2,T>
{

    nc_vec(T x_ = 0.0, T y_ = 0.0) : x(x_), y(y_) {};

    template<class T2>
    nc_vec(const nc_vec<2,T2> & v)
    {
        x = (T)v.x;
        y = (T)v.y;
    }

    void print(const char* text = 0) const
    {

        if (text)
            fprintf(stderr,"%s\n",text);
        std::cerr << x << " ";
        std::cerr << y ;
        std::cerr << std::endl;
    };

    bool operator<(const nc_vec <2,T>& rhs ) const
    {
        if (x!=rhs.x)
            return (x<rhs.x);
        else
            return (y<rhs.y);
    }

    bool operator==(const nc_vec<2,T> & rhs) const
    {
        return ((x==rhs.x) && (y==rhs.y));
    }

    bool operator!=(const nc_vec<2,T> & rhs) const
    {
       return !((*this)==rhs);
    }

    T& operator[](int index)
    {
        return (&x)[index];
    };

    const T& operator[](int index) const
    {
        return (&x)[index];
    };

    nc_vec<2,T> operator-(const nc_vec<2,T> & vec) const
    {
        return nc_vec(x-vec.x,y-vec.y);
    }

    nc_vec<2,T> operator+(const nc_vec<2,T> & vec) const
    {
        return nc_vec(x+vec.x,y+vec.y);
    }

    T norm()
    {
        return sqrt(x*x+y*y);
    };

    T x,y;
};

template <class T>
struct nc_vec <3,T>
{

    nc_vec(T x_ = 0.0, T y_ = 0.0, T z_ = 0.0) : x(x_), y(y_) , z(z_) {};

    template<class T2>
    nc_vec(const nc_vec<3,T2> & v)
    {
        x = (T)v.x;
        y = (T)v.y;
        z = (T)v.z;
    }

    bool operator!=(const nc_vec <3,T>& rhs ) const
    {
        return !((*this)==rhs);
    }

    bool operator<(const nc_vec <3,T>& rhs ) const
    {
        if (x!=rhs.x)
            return (x<rhs.x);
        else
        {
            if (y!=rhs.y)
                return (y<rhs.y);
            else
                return (z<rhs.z);
        }
    }

    T& operator[](int index)
    {
        return (&x)[index];
    };

    const T& operator[](int index) const
    {
        return (&x)[index];
    };

    void print(const char* text = 0) const
    {

        if (text)
            fprintf(stderr,"%s\n",text);
        std::cerr << x << " ";
        std::cerr << y << " ";
        std::cerr << z ;
        std::cerr << std::endl;
    };

    nc_vec<3,T> operator-(const nc_vec<3,T> & vec) const
    {
        return nc_vec(x-vec.x,y-vec.y,z-vec.z);
    }

    nc_vec<3,T> & operator+=(const nc_vec<3,T> & vec)
    {
        x+=vec.x;
        y+=vec.y;
        z+=vec.z;
        return *this;
    }

    nc_vec<3,T> & operator-=(const nc_vec<3,T> & vec)
    {
        x-=vec.x;
        y-=vec.y;
        z-=vec.z;
        return *this;
    }

    nc_vec<3,T> & operator/=(const T & f)
    {
        x/=f;
        y/=f;
        z/=f;
        return *this;
    }

    nc_vec<3,T> operator+(const nc_vec<3,T> & vec)
    {
        return nc_vec(x+vec.x,y+vec.y,z+vec.z);
    }

    bool operator==(const nc_vec<3,T> & vec) const
    {
        return ((x==vec.x) && (y==vec.y) && (z==vec.z));
    }

    nc_vec<3,T> normalize()
    {
        T n = (T)(1.0/norm());
        return nc_vec<3,T>(n*x,n*y,n*z);
    };

    nc_vec<2,T> xy() const
    {

        return nc_vec<2,T>(x,y);
    }

    T norm()
    {
        return sqrt(x*x+y*y+z*z);
    };

    T x,y,z;
};

template <class T>
struct nc_vec <4,T>
{

    nc_vec(T x_ = 0.0, T y_ = 0.0, T z_ = 0.0, T w_ = 0.0) : x(x_), y(y_) , z(z_) , w(w_) {};

    nc_vec(nc_vec <3,T> & v , T w_ ) : x(v.x), y(v.y) , z(v.z) , w(w_) {};

    template<class T2>
    nc_vec(const nc_vec<4,T2> & v)
    {
        x = (T)v.x;
        y = (T)v.y;
        z = (T)v.z;
        w = (T)v.w;
    }

    T& operator[](int index)
    {
        return (&x)[index];
    };

    const T& operator[](int index) const
    {
        return (&x)[index];
    };

    void print(const char* text = 0) const
    {

        if (text)
            fprintf(stderr,"%s\n",text);
        std::cerr << x << " ";
        std::cerr << y << " ";
        std::cerr << z << " ";
        std::cerr << w ;
        std::cerr << std::endl;
    };

    nc_vec<4,T> & operator/=(const T & f)
    {
        x/=f;
        y/=f;
        z/=f;
        w/=f;
        return *this;
    }

    nc_vec<4,T> & operator+=(const nc_vec<4,T> & vec)
    {
        x+=vec.x;
        y+=vec.y;
        z+=vec.z;
        w+=vec.w;
        return *this;
    }

    nc_vec<4,T> & operator-=(const nc_vec<4,T> & vec)
    {
        x-=vec.x;
        y-=vec.y;
        z-=vec.z;
        w-=vec.w;
        return *this;
    }

    nc_vec<4,T> operator/(const T & f)
    {
        return nc_vec(x/f,y/f,z/f,w/f);
    }

    nc_vec<4,T> operator+(const nc_vec<4,T> & vec)
    {
        return nc_vec(x+vec.x,y+vec.y,z+vec.z,w+vec.w);
    }

    nc_vec<4,T> operator-(const nc_vec<4,T> & vec)
    {
        return nc_vec(x-vec.x,y-vec.y,z-vec.z,w-vec.w);
    }

	nc_vec<4,T> operator>>(const int & shift) const
    {
        nc_vec<4,T> out;

        out.x = x>>shift;
        out.y = y>>shift;
        out.z = z>>shift;
        out.w = w>>shift;

        return out;
    }

	nc_vec<4,T> operator<<(const int & shift) const
    {
        nc_vec<4,T> out;

        out.x = x<<shift;
        out.y = y<<shift;
        out.z = z<<shift;
        out.w = w<<shift;

        return out;
    }

    bool operator==(const nc_vec<4,T> & vec) const
    {
        return ((x==vec.x) && (y==vec.y) && (z==vec.z) && (w==vec.w));
    }

    bool operator!=(const nc_vec<4,T> & vec) const
    {
        return !((*this)==vec);
	}

    nc_vec<3,T> xyz() const
    {

        return nc_vec<3,T>(x,y,z);
    }

    nc_vec<2,T> yz() const
    {

        return nc_vec<2,T>(y,z);
    }

    nc_vec<4,T> normalize()
    {
        T n = (T)(1.0/norm());
        return nc_vec<4,T>(n*x,n*y,n*z,n*w);
    };

    T norm()
    {
        return sqrt(x*x+y*y+z*z+w*w);
    };

    T x,y,z,w;
};

template <unsigned int N, class T>
nc_vec<N,T> operator*(const T & fact, const nc_vec<N,T> & vec)
{

    nc_vec<N,T> out;
    for (unsigned int i=0;i<N;++i)
		out[i] = fact*vec[i];
    return out;
}

template <class T>
struct nc_m3x3
{

    nc_m3x3() {};

    void setIdentity()
    {
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j)
                col[i][j] = (i==j? 1.0 : 0.0);
    }

    template<class T2>
    nc_m3x3(const nc_m3x3<T2> & m)
    {
        for (int i=0; i<3; ++i)
            col[i] = m[i];
    }

    nc_vec<3,T>& operator[](int index)
    {
        return col[index];
    };

    const nc_vec<3,T>& operator[](int index) const
    {
        return col[index];
    };

    nc_vec<3,T> operator*(const nc_vec<3,T>& vec) const
    {

        nc_vec<3,T> out;
        for (int i=0; i<3; ++i)
            out+=vec[i]*col[i];
        return out;
    };

    nc_m3x3<T> operator*(const nc_m3x3<T>& mat) const
    {

        nc_m3x3<T> out;
        for (int i=0; i<3; ++i)
            out[i] = (*this)*mat[i];
        return out;
    };

    nc_m3x3<T> inverse() const
    {
//!scenix
#if 0
        nvmath::Matnnt<3,T> tmp = *((nvmath::Matnnt<3,T>*)(this));
        tmp.invert();
        nc_m3x3<T> out = *((nc_m3x3<T>*)(&tmp));
        return out;
#endif

//! vnl
#if 0
        nc_m3x3<T> out;

        vnl_matrix_fixed<double,3,3> tmp;
        for (unsigned int i=0; i<3; ++i)
            for (unsigned int j=0; j<3; ++j)
                tmp(j,i) = col[i][j];

        vnl_matrix<double> tmpi = vnl_matrix_inverse<double>(tmp);

        for (unsigned int i=0; i<3; ++i)
            for (unsigned int j=0; j<3; ++j)
                out[i][j] = tmpi(j,i);

        return out;
#endif

        nc_m3x3<T> out;

        T adj00 =   ( col[1][1] * col[2][2] - col[1][2] * col[2][1] );
        T adj10 = - ( col[1][0] * col[2][2] - col[1][2] * col[2][0] );
        T adj20 =   ( col[1][0] * col[2][1] - col[1][1] * col[2][0] );
        T det = col[0][0] * adj00 + col[0][1] * adj10 + col[0][2] * adj20;
        bool ok = ( std::numeric_limits<T>::epsilon() < abs( det ) );
        //if ( !ok )
        //    fprintf(stderr,"Warning: Badly conditioned 3x3 matrix inverse...\n");

        T invDet = T(1) / det;
        out[0][0] =   adj00 * invDet;
        out[0][1] = - ( col[0][1] * col[2][2] - col[0][2] * col[2][1] ) * invDet;
        out[0][2] =   ( col[0][1] * col[1][2] - col[0][2] * col[1][1] ) * invDet;
        out[1][0] =   adj10 * invDet;
        out[1][1] =   ( col[0][0] * col[2][2] - col[0][2] * col[2][0] ) * invDet;
        out[1][2] = - ( col[0][0] * col[1][2] - col[0][2] * col[1][0] ) * invDet;
        out[2][0] =   adj20 * invDet;
        out[2][1] = - ( col[0][0] * col[2][1] - col[0][1] * col[2][0] ) * invDet;
        out[2][2] =   ( col[0][0] * col[1][1] - col[0][1] * col[1][0] ) * invDet;

        return out;
    }

    static nc_m3x3 rotX(double angle)
    {
        nc_m3x3 out;
        out.setIdentity();
        out[1][1] = (T) cos(angle);
        out[1][2] = (T) sin(angle);
        out[2][1] = (T)-sin(angle);
        out[2][2] = (T) cos(angle);
        return out;
    };

    static nc_m3x3 rotY(double angle)
    {
        nc_m3x3 out;
        out.setIdentity();
        out[0][0] = (T) cos(angle);
        out[0][2] = (T) sin(angle);
        out[2][0] = (T)-sin(angle);
        out[2][2] = (T) cos(angle);
        return out;
    };

    static nc_m3x3 rotZ(double angle)
    {
        nc_m3x3 out;
        out.setIdentity();
        out[0][0] = (T) cos(angle);
        out[0][1] = (T) sin(angle);
        out[1][0] = (T)-sin(angle);
        out[1][1] = (T) cos(angle);
        return out;
    };

    void print(const char* text = 0) const
    {

        if (text)
            fprintf(stderr,"%s\n",text);
        for (unsigned int r=0; r<3; ++r)
        {
            for (unsigned int c=0; c<3; ++c)
            {
                std::cerr << col[c][r] << " ";
            }
            std::cerr << std::endl;
        }
    };

    nc_vec<3,T> col[3];
};

template <class T>
nc_m3x3<T> operator*(const T & fact, const nc_m3x3<T> & mat)
{
    nc_m3x3<T> out;
    for (unsigned int i=0;i<3;++i)
		out[i] = fact*mat[i];
    return out;
}


template <class T>
struct nc_m4x4
{

    nc_m4x4() {};

    void setIdentity()
    {
        for (int i=0; i<4; ++i)
            for (int j=0; j<4; ++j)
                col[i][j] = (T)(i==j? 1.0 : 0.0);
    }

    static nc_m4x4 getIdentity()
    {
        nc_m4x4 tmp;
        tmp.setIdentity();
        return tmp;
    }

    static nc_m4x4 ortho(T l , T r , T b , T t , T n , T f)
    {
        nc_m4x4 tmp;
        tmp.setIdentity();
		tmp[0][0] = 2.0/(r-l);
		tmp[3][0] = -(r+l)/(r-l);
		tmp[1][1] = 2.0/(t-b);
		tmp[3][1] = -(t+b)/(t-b);
		tmp[2][2] = 2.0/(f-n);
		tmp[3][2] = -(f+n)/(f-n);

        return tmp;
    }

    template<class T2>
    nc_m4x4(const nc_m4x4<T2> & m)
    {
        for (int i=0; i<4; ++i)
            col[i] = m[i];
    }


    /*
    nvmath::Matnnt<4,T> nc_m4x4(const nc_m4x4 & m)
    {
        for (int i=0; i<4; ++i)
            col[i] = m[i];
    }
    */


    nc_vec<4,T>& operator[](int index)
    {
        return col[index];
    };

    const nc_vec<4,T>& operator[](int index) const
    {
        return col[index];
    };

    nc_vec<4,T> operator*(const nc_vec<4,T>& vec) const
    {

        nc_vec<4,T> out;
        for (int i=0; i<4; ++i)
            out+=vec[i]*col[i];
        return out;
    };

    nc_m4x4<T> operator*(const nc_m4x4<T>& mat) const
    {

        nc_m4x4<T> out;
        for (int i=0; i<4; ++i)
            out[i] = (*this)*mat[i];
        return out;
    };

    nc_m4x4<T> inverse() const
    {
//!scenix
#if 0
        nvmath::Matnnt<4,T> tmp = *((nvmath::Matnnt<4,T>*)(this));
        tmp.invert();
        nc_m4x4<T> out = *((nc_m4x4<T>*)(&tmp));
        return out;
#endif

//! vnl
#if 0
        nc_m4x4<T> out;

        vnl_matrix_fixed<double,4,4> tmp;
        for (unsigned int i=0; i<4; ++i)
            for (unsigned int j=0; j<4; ++j)
                tmp(j,i) = col[i][j];

        vnl_matrix<double> tmpi = vnl_matrix_inverse<double>(tmp);

        for (unsigned int i=0; i<4; ++i)
            for (unsigned int j=0; j<4; ++j)
                out[i][j] = (T)tmpi(j,i);

        return out;
#endif

        nc_m4x4<T> out;

        T s0 = col[0][0] * col[1][1] - col[0][1] * col[1][0];   T c5 = col[2][2] * col[3][3] - col[2][3] * col[3][2];
        T s1 = col[0][0] * col[1][2] - col[0][2] * col[1][0];   T c4 = col[2][1] * col[3][3] - col[2][3] * col[3][1];
        T s2 = col[0][0] * col[1][3] - col[0][3] * col[1][0];   T c3 = col[2][1] * col[3][2] - col[2][2] * col[3][1];
        T s3 = col[0][1] * col[1][2] - col[0][2] * col[1][1];   T c2 = col[2][0] * col[3][3] - col[2][3] * col[3][0];
        T s4 = col[0][1] * col[1][3] - col[0][3] * col[1][1];   T c1 = col[2][0] * col[3][2] - col[2][2] * col[3][0];
        T s5 = col[0][2] * col[1][3] - col[0][3] * col[1][2];   T c0 = col[2][0] * col[3][1] - col[2][1] * col[3][0];
        T det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

        bool ok = ( std::numeric_limits<T>::epsilon() < std::abs( det ) );
        //if ( !ok )
        //    fprintf(stderr,"Warning: Badly conditioned 4x4 matrix inverse...\n");

        T invDet = T(1) / det;
        out[0][0] = (   col[1][1] * c5 - col[1][2] * c4 + col[1][3] * c3 ) * invDet;
        out[0][1] = ( - col[0][1] * c5 + col[0][2] * c4 - col[0][3] * c3 ) * invDet;
        out[0][2] = (   col[3][1] * s5 - col[3][2] * s4 + col[3][3] * s3 ) * invDet;
        out[0][3] = ( - col[2][1] * s5 + col[2][2] * s4 - col[2][3] * s3 ) * invDet;
        out[1][0] = ( - col[1][0] * c5 + col[1][2] * c2 - col[1][3] * c1 ) * invDet;
        out[1][1] = (   col[0][0] * c5 - col[0][2] * c2 + col[0][3] * c1 ) * invDet;
        out[1][2] = ( - col[3][0] * s5 + col[3][2] * s2 - col[3][3] * s1 ) * invDet;
        out[1][3] = (   col[2][0] * s5 - col[2][2] * s2 + col[2][3] * s1 ) * invDet;
        out[2][0] = (   col[1][0] * c4 - col[1][1] * c2 + col[1][3] * c0 ) * invDet;
        out[2][1] = ( - col[0][0] * c4 + col[0][1] * c2 - col[0][3] * c0 ) * invDet;
        out[2][2] = (   col[3][0] * s4 - col[3][1] * s2 + col[3][3] * s0 ) * invDet;
        out[2][3] = ( - col[2][0] * s4 + col[2][1] * s2 - col[2][3] * s0 ) * invDet;
        out[3][0] = ( - col[1][0] * c3 + col[1][1] * c1 - col[1][2] * c0 ) * invDet;
        out[3][1] = (   col[0][0] * c3 - col[0][1] * c1 + col[0][2] * c0 ) * invDet;
        out[3][2] = ( - col[3][0] * s3 + col[3][1] * s1 - col[3][2] * s0 ) * invDet;
        out[3][3] = (   col[2][0] * s3 - col[2][1] * s1 + col[2][2] * s0 ) * invDet;

        return out;
    }

    nc_m4x4<T> transpose() const
    {

        nc_m4x4<T> out;

        for (unsigned int i=0; i<4; ++i)
            for (unsigned int j=0; j<4; ++j)
                out[i][j] = col[j][i];
        return out;
    }

    void print(const char* text = 0) const
    {

        if (text)
            fprintf(stderr,"%s\n",text);
        for (unsigned int r=0; r<4; ++r)
        {
            for (unsigned int c=0; c<4; ++c)
            {
                std::cerr << col[c][r] << " ";
            }
            std::cerr << std::endl;
        }
    };

    nc_vec<4,T> col[4];
};


template <class T>
struct nc_RT : public nc_m4x4<T>
{
    using nc_m4x4<T>::col;

	nc_RT() {
		this->setIdentity();
	}

    template<class T2>
    nc_RT(const nc_RT<T2> & m)
    {
        for (int i=0; i<4; ++i)
            col[i] = m[i];
    }

    static nc_RT getIdentity()
    {
        nc_RT tmp;
        tmp.setIdentity();
        return tmp;
    }


	nc_RT<T> inverse() const
    {
        nc_RT<T> out;
        out.setIdentity();

        nc_m3x3<T>  rot;
        nc_vec<3,T> trans;
        for (int i=0;i<3;++i)
        {
            trans[i] = (*this)[3][i];
            for (int j=0;j<3;++j)
                rot[i][j] = (*this)[j][i];
        }

        trans = T(-1.0)*rot*trans;

        for (int i=0;i<3;++i)
        {
            out[3][i] = trans[i];
            for (int j=0;j<3;++j)
                out[i][j] = rot[i][j];
        }

        return out;
    }

    static nc_RT<T> translate(nc_vec<3,double> tr)
    {
        nc_RT<T> out;
        out.setIdentity();
        out[3][0] = (T)tr[0];
        out[3][1] = (T)tr[1];
        out[3][2] = (T)tr[2];
        return out;
    };

    static nc_RT<T> translate(double x = 0.0, double y = 0.0, double z = 0.0)
    {
        nc_RT<T> out;
        out.setIdentity();
        out[3][0] = (T)x;
        out[3][1] = (T)y;
        out[3][2] = (T)z;
        return out;
    };

    static nc_RT<T> scale(double x = 1.0, double y = 1.0, double z = 1.0, double w = 1.0)
    {
        nc_RT<T> out;
        out.setIdentity();
        out[0][0] = (T)x;
        out[1][1] = (T)y;
        out[2][2] = (T)z;
        out[3][3] = (T)w;
        return out;
    };

    static nc_RT<T> rotX(double angle)
    {
        nc_RT<T> out;
        out.setIdentity();

        nc_m3x3<T> rot = nc_m3x3<T>::rotX(angle);
        for (int i=0;i<3;++i)
            for (int j=0;j<3;++j)
                out[i][j] = rot[i][j];

        return out;
    };

    static nc_RT<T> rotY(double angle)
    {
        nc_RT<T> out;
        out.setIdentity();

        nc_m3x3<T> rot = nc_m3x3<T>::rotY(angle);
        for (int i=0;i<3;++i)
            for (int j=0;j<3;++j)
                out[i][j] = rot[i][j];

        return out;
    };

    static nc_RT<T> rotZ(double angle)
    {
        nc_RT<T> out;
        out.setIdentity();

        nc_m3x3<T> rot = nc_m3x3<T>::rotZ(angle);
        for (int i=0;i<3;++i)
            for (int j=0;j<3;++j)
                out[i][j] = rot[i][j];

        return out;
    };

    nc_vec<3,T> operator*(const nc_vec<3,T>& vec) const
    {
        nc_vec<3,T> out;
        for (int i=0; i<3; ++i)
            out+=vec[i]*col[i].xyz();
        out+= col[3].xyz();
        return out;
    };

    nc_vec<4,T> operator*(const nc_vec<4,T>& vec) const
    {

        nc_vec<4,T> out;
        for (int i=0; i<4; ++i)
            out+=vec[i]*col[i];
        return out;
    };

    nc_RT<T> operator*(const nc_RT<T>& mat) const
    {
        nc_RT<T> out;
        for (int i=0; i<4; ++i)
            out[i] = (*this)*mat[i];
        return out;
    };

    nc_m4x4<T> operator*(const nc_m4x4<T>& mat) const
    {
        nc_m4x4<T> out;
        for (int i=0; i<4; ++i)
            out[i] = (*this)*mat[i];
        return out;
    };


};


template <class T>
bool operator<( const nc_vec <2,T>& lhs, const nc_vec <2,T>& rhs )
{
    if (lhs.x!=rhs.x)
        return (lhs.x<rhs.x);
    else
        return (lhs.y<rhs.y);
}


template<class T>
nc_vec<3,T> cross (const nc_vec<3,T> & a , const nc_vec<3,T> & b)
{
    return nc_vec<3,T>(a.y*b.z-a.z*b.y , a.z*b.x-a.x*b.z , a.x*b.y-a.y*b.x);
};

template<unsigned int N, class T>
T dot (const nc_vec<N,T> & a , const nc_vec<N,T> & b)
{
	T out = (T)0.0;
	for (unsigned int i=0;i<N;++i)
		out+=a[i]*b[i];
    return out;
};

template<unsigned int N, class T>
nc_vec<N,T> min(const nc_vec<N,T> & a,const nc_vec<N,T> & b)
{
    nc_vec<N,T> out;
    for (unsigned int i=0;i<N;++i)
		out[i] = std::min(a[i],b[i]);

    return out;
}

template<unsigned int N, class T>
nc_vec<N,T> max(const nc_vec<N,T> & a,const nc_vec<N,T> & b)
{
    nc_vec<N,T> out;
    for (unsigned int i=0;i<N;++i)
		out[i] = std::max(a[i],b[i]);

    return out;
}


struct v3f4ub {

    v3f4ub(nc_vec<3,float> pos_ = nc_vec<3,float>(0.0f,0.0f,0.0f), nc_vec<4,unsigned char> col_ = nc_vec<4,unsigned char>(0,0,0,0))
    {
        x = pos_.x;
        y = pos_.y;
        z = pos_.z;
        r = col_.x;
        g = col_.y;
        b = col_.z;
        a = col_.w;
    }

    nc_vec<3,float>& pos() const
    {
        return *((nc_vec<3,float>*)&x);
    }

    nc_vec<4,unsigned char>& color() const
    {
        return *((nc_vec<4,unsigned char>*)&r);
    }

    float x;
    float y;
    float z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

template <class T>
nc_m4x4<T> PGLtoP(nc_m4x4<T> PGL , int w, int h) {
    return (nc_RT<T>::scale(w,h,1.0,1.0))*(nc_RT<T>::translate(+0.5,+0.5,+0.5))*(nc_RT<T>::scale(0.5,0.5,0.5))*PGL;
}

template <class T>
nc_m4x4<T> PGLtoPTEX(nc_m4x4<T> PGL) {
    return (nc_RT<T>::translate(+0.5,+0.5,+0.5))*(nc_RT<T>::scale(0.5,0.5,0.5))*PGL;
}

template <class T>
nc_m4x4<T> PtoPGL(nc_m4x4<T> P , int w, int h) {
    return (nc_RT<T>::scale(2.0,2.0,2.0))*(nc_RT<T>::translate(-0.5,-0.5,-0.5))*(nc_RT<T>::scale(1.0/w,1.0/h,1.0,1.0))*P;
}

template <class T>
nc_m4x4<T> PtoPTEX(nc_m4x4<T> P , int w, int h) {
    return nc_RT<T>::scale(1.0/w,1.0/h,1.0,1.0)*P;
}

template <class T>
nc_m4x4<T> PTEXtoP(nc_m4x4<T> PTEX , int w, int h) {
    return nc_RT<T>::scale(w,h,1.0,1.0)*PTEX;
}

template <class T>
nc_m4x4<T> PTEXtoPGL(nc_m4x4<T> PTEX) {
    return (nc_RT<T>::scale(2.0,2.0,2.0))*(nc_RT<T>::translate(-0.5,-0.5,-0.5))*PTEX;
}



typedef nc_vec	<2,float>			v2f;
typedef nc_vec	<3,float>			v3f;
typedef nc_vec	<4,float>			v4f;

typedef nc_vec	<2,double>			v2d;
typedef nc_vec	<3,double>			v3d;
typedef nc_vec	<4,double>			v4d;

typedef nc_vec	<2,int>				v2i;
typedef nc_vec	<3,int>				v3i;
typedef nc_vec	<4,int>				v4i;

typedef nc_vec	<2,unsigned int>	v2ui;
typedef nc_vec	<3,unsigned int>	v3ui;
typedef nc_vec	<4,unsigned int>	v4ui;

typedef nc_vec	<2,unsigned short>	v2us;
typedef nc_vec	<3,unsigned short>	v3us;
typedef nc_vec	<4,unsigned short>	v4us;

typedef nc_vec	<2,unsigned char>	v2ub;
typedef nc_vec	<3,unsigned char>	v3ub;
typedef nc_vec	<4,unsigned char>	v4ub;

typedef nc_vec	<2,bool>			v2bool;
typedef nc_vec	<3,bool>			v3bool;
typedef nc_vec	<4,bool>			v4bool;

typedef nc_vec	<2,long long>		v2ll;
typedef nc_vec	<3,long long>		v3ll;
typedef nc_vec	<4,long long>		v4ll;

typedef nc_m3x3	<float>	 		    m3x3f;
typedef nc_m4x4	<float>			    m4x4f;

typedef nc_m3x3	<double>			m3x3d;
typedef nc_m4x4	<double>			m4x4d;

typedef nc_RT	<float>			    RTf;
typedef nc_RT	<double>			RTd;

};

#endif
