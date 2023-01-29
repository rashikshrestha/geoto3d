#ifndef __NC_COMMON__
#define __NC_COMMON__

#include "libpgf/PGFimage.h"

#if defined(_WIN32)
#include <windows.h>
#define strcasecmp(s1, s2)	stricmp(s1, s2)
#define popen(x,y) _popen(x,y)
#define pclose(x)  _pclose(x)
#else
#include <libgen.h>
#include <dirent.h>
#include <glob.h>
#endif

#undef min
#undef max
#undef round

#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <sstream>
#include <stdexcept>
#include <fcntl.h>
#include <cmath>
#include <algorithm>
#include <string>

#include "ncmath.h"

#include "triangle.h"
#include "tetgen.h"
#include "nccalibration.h"

#include "Magick++.h"

#define MAX_CAM         25

class ncCalibrationSet;
class ncPoseList;

class ncDebug {
public:
    ncDebug(std::string name);
    ~ncDebug();

    static bool use_tabs;
    static int  nr_tabs;
    std::string name;
};

//#define NCDEBUG1() char debug_str[1024]; sprintf(debug_str, "%s()", __FUNCTION__); ncDebug debug_instance(std::string(debug_str));
//#define NCDEBUG2(...) char debug_str[1024]; { char arg_str[1024];  sprintf(arg_str, __VA_ARGS__); sprintf(debug_str, "%s(%s)", __FUNCTION__ ,  arg_str); } ncDebug debug_instance(std::string(debug_str));

template <class T>
class ncPtr {
public:
    ncPtr(T* ptr = 0) : ptr(ptr) {
        //fprintf(stderr,"ncPtr(%p)\n",ptr);
    };

    ~ncPtr() {
        //fprintf(stderr,"~ncPtr(%p)\n",ptr);
        reset();
    };

    T*& get() {
        //fprintf(stderr,"~get(%p)\n",ptr);
        return ptr;
    };

    operator bool () {
        return (ptr!=0);
    }

    operator T*& () {
        return ptr;
    }

    operator T& () {
        return *ptr;
    }

    T*& operator->() {
        return ptr;
    }

    void operator=(T* ptr_) {
        //fprintf(stderr,"operator=(%p)\n",ptr_);
        reset();
        ptr = ptr_;
    }

    void reset() {
        //fprintf(stderr,"reset(%p)\n",ptr);
        if (ptr)
        {
            delete ptr;
            ptr = 0;
        }
    }

private:
    T* ptr;
};

template <class T>
struct ncImageBuffer {

	ncImageBuffer() : width(0),height(0),depth(0),data(0) {
		if (!IM_initialized)
		{
			Magick::InitializeMagick("");
			IM_initialized = true;
		}
	};

	ncImageBuffer(const ncImageBuffer& imbuf) {
		*this=imbuf;
	}

    ncImageBuffer(unsigned int w, unsigned int h = 1, unsigned int d = 1) : width(w),height(h),depth(d) {
		if (!IM_initialized)
		{
			Magick::InitializeMagick("");
			IM_initialized = true;
		}

		data = w*h*d>0? (T*)malloc(w*h*d*sizeof(T)) : 0;
	};

	~ncImageBuffer() {
    if (data)
    {
        free(data);
        data = 0;
    }
    width = height = depth = 0;
}



	ncImageBuffer<T>& operator=(const ncImageBuffer& imbuf) {
		if (data)
		{
			free(data);
			data = 0;
		}
		width   = imbuf.width;
		height  = imbuf.height;
		depth   = imbuf.depth;

		int nrpixels = width*height*depth;
		if (nrpixels>0)
		{
			data    = (T*)malloc(nrpixels*sizeof(T));
			memcpy(data,imbuf.data,nrpixels*sizeof(T));
		}
		return *this;
	}



    void resize(unsigned int w, unsigned int h = 1, unsigned int d = 1) {
    if ((width!=w) || (height!=h) || (depth!=d))
    {
        width   = w;
        height  = h;
        depth   = d;
        if (data)
        {
            free(data);
            data = 0;
        }
        int nrpixels = width*height*depth;
        if (nrpixels>0)
           data    = (T*)malloc(nrpixels*sizeof(T));
    }
}


    void fill(T fval) {
		for (unsigned int i=0;i<width*height*depth;++i)
			data[i] = fval;
	}

    T& operator[] (unsigned int i) { return data[i]; }
    T& operator() (unsigned int x, unsigned int y = 0, unsigned int z = 0) { return data[(z*height+y)*width+x]; }

    unsigned int width;
    unsigned int height;
    unsigned int depth;
    T*           data;

    static bool IM_initialized;
};

template<class T>
bool ncImageBuffer<T>::IM_initialized = false;

struct cameraInfo {

    cameraInfo() : regexp("") , color(true) , jpg(false) , hdr(false) , valid(false) , width(0) , height(0) {}

	std::string regexp;
	bool		color;
	bool		jpg;
	bool		hdr;
	bool		valid;
	int         width;
	int         height;
};

class ImageInfo {

public:
    ImageInfo();
    ~ImageInfo();
    ImageInfo(std::string im_dir , ncCalibrationSet& camset , ncPoseList& poses   );
    ImageInfo(std::string im_dir , std::string camsetfile   , std::string posefile);

public:
    void        setup(std::string im_dir , ncCalibrationSet& camset , ncPoseList& poses   );
    void        setup(std::string im_dir , std::string camsetfile   , std::string posefile);
    bool        load( ncImageBuffer<ncc::v4ub>& imbuf , unsigned int cid, unsigned int pid , unsigned int level = 0, int border = 1 );
    bool        loadThumbnail( ncImageBuffer<ncc::v4ub>& imbuf , unsigned int cid, unsigned int pid , unsigned int minimum_nr_of_pixels , int border = 1 );
    std::string getFileName(unsigned int cid,unsigned int pid);
    void        print();

public:
    std::string directory;
	cameraInfo  cam_info[MAX_CAM];
	bool        valid;

};


namespace ncc {

    std::string getDirName(std::string fname);
    std::string getFileName(std::string fname);

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);

	std::vector<std::string>
	getFiles(std::string path, std::string pattern = "*");

    bool
    getAnyImageFile(std::string image_dir , unsigned int cid , unsigned int pid_min , unsigned int pid_max , std::string & fname);

	bool
	getFirstFile(std::string dname , std::string & fname , bool full_path);

	bool
	getFirstFile(std::string dname , std::string pattern , std::string & fname);

	bool
	checkFile(std::string fname, std::string* msg = 0);

    std::string
    getFileExt(std::string fname);

    std::string
    changeFileExt(std::string fname , std::string new_ext);

    std::string
    getFileBaseName(std::string fname);

    bool
    checkFileExt(std::string fname,std::string ext);

	bool
	checkDir(std::string dname, std::string* msg = 0);

	bool
	createDir(std::string dname, std::string* msg = 0);

	unsigned int isHDR(std::string fname);
	unsigned int isGray(std::string fname);
    bool readPGF( std::string   fname, ncImageBuffer<unsigned char>	    & imbuf, int level	= 0	, bool presized = false );
    bool readPGF( std::string   fname, ncImageBuffer<unsigned short>	& imbuf, int level	= 0	, bool presized = false );
    bool readPGF( std::string   fname, ncImageBuffer<ncc::v3ub>     	& imbuf, int level	= 0	, bool presized = false );
    bool readPGF( std::string   fname, ncImageBuffer<ncc::v4ub>     	& imbuf, int level	= 0	, bool presized = false );
	bool readPGF( std::string   fname, ncImageBuffer<ncc::v3us>     	& imbuf, int level	= 0	, bool presized = false );
	bool writePGF(std::string   fname, ncImageBuffer<ncc::v3ub>     	& imbuf, int levels		, int quality		    );

	bool readImage(std::string  fname, ncImageBuffer<ncc::v3ub>     & imbuf, int level	= 0	, bool flip = false);
	bool readImage(std::string  fname, ncImageBuffer<ncc::v4ub>     & imbuf, int level	= 0	, bool flip = false);
	bool readImage(std::string  fname, ncImageBuffer<unsigned char> & imbuf, int level	= 0	, bool flip = false);

	bool writeImage(std::string fname, ncImageBuffer<ncc::v3ub> & imbuf, bool flip = false);
	bool writeImage(std::string fname, ncImageBuffer<ncc::v4ub> & imbuf, bool flip = false);
	bool writeImage(std::string fname, ncImageBuffer<unsigned char  > & imbuf, bool flip = false);
	bool writeImage(std::string fname, ncImageBuffer<unsigned short > & imbuf , unsigned short  minval = 0x0000 , unsigned short    maxval = 0xFFFF , bool flip = false);
	bool writeImage(std::string fname, ncImageBuffer<float          > & imbuf , float           minval = 0.0f   , float             maxval = 1.0f   , bool flip = false);

	void writePLY(std::string fname ,std::vector<ncc::v3f4ub> data);


	template <class T>
    void
    cleanList(std::vector<T> & vec) {
        typename std::vector<T>::iterator it;
        std::sort(vec.begin(),vec.end());
        it = std::unique(vec.begin(),vec.end());
        vec.resize(it-vec.begin());
    }

    void
    delaunay2D(const std::vector<ncc::v2d> & vin, std::vector<ncc::v3i> & vout);

    void
    delaunay3D(const std::vector<ncc::v3d> & vin, std::vector<ncc::v4i> & vout);

    void
    rectify(ncCalibration cal1_in , ncCalibration cal2_in, ncCalibration & cal1_out , ncCalibration & cal2_out, int imw, int imh, double near, double far);

	ncc::v4d
	fitToCanvas(double w_in, double h_in, double w_out, double h_out);

	ncc::v4d
	fitToCanvasGL(double w_in, double h_in, double w_out, double h_out);

	ncc::RTd
	lookAt(ncc::v3d eye,ncc::v3d center,ncc::v3d up);

	template<class T>
	void swap(T*& a, T*& b)
	{
	    T* tmp = a;
	    a = b;
	    b = tmp;
	}

	template<class T1,class T2>
	T1 cast(T2& o)
	{
	    return *((T1*)(&(o)));
	}

	template<class T1,class T2>
	T1 cast(T2 o)
	{
	    return *((T1*)(&(o)));
	}

	void ClearBorders(unsigned short *rgb, int sx, int sy, int w);
	void debayer(const unsigned short *bayer, unsigned short *rgb, int sx, int sy);
};

#undef NoError
#endif
