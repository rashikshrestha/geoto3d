#ifndef __NC_VOXEL_H__
#define __NC_VOXEL_H__

#include <fstream>
#include <vector>
#include "ncmath.h"

#include "ncgl.h"
#include "nccalibration.h"
#include "ncfbo.h"
#include "ncpbo.h"
#include "ncmesh.h"
#include "ncprogram.h"
#include "ncteximage.h"
#include "ncconfig.h"

#include "nccommon.h"

#define NEG_X 0x01
#define POS_X 0x02
#define NEG_Y 0x04
#define POS_Y 0x08
#define NEG_Z 0x10
#define POS_Z 0x20


#define NC_CHILD    0x01
#define NC_CARVE    0x02
#define NC_RGBM     0x04
#define NC_COLOR    0x08
#define NC_PTR      0x10

struct ncBoundingBox {

    ncBoundingBox() {
        pos  = ncc::v3d(0.0,0.0,0.0);
        size = ncc::v3d(0.0,0.0,0.0);
    }
    ncBoundingBox(ncc::v3d pos_, ncc::v3d size_) {
        pos  = pos_;
        size = size_;
    }
    ncc::v3d pos;
    ncc::v3d size;
};

template<unsigned int N>
struct ncAttribute {
};

template<>
struct ncAttribute<NC_CHILD> {
    ncAttribute (unsigned int child = 0) : child(child) {};

    operator unsigned int& () {
        return child;
    }

    unsigned int child;

};

template<>
struct ncAttribute<NC_CARVE> {
    ncAttribute (unsigned int carve = 0) : carve(carve) {};

    operator unsigned int& () {
        return carve;
    }

    unsigned int carve;

};

template<>
struct ncAttribute<NC_RGBM>  {

    ncAttribute (unsigned char r = 0 , unsigned char g = 0 , unsigned char b = 0 , unsigned char mask = 0) : r(r) , g(g) , b(b) , mask(mask) {};

    unsigned char mask;
    unsigned char b;
    unsigned char g;
    unsigned char r;
};
typedef ncAttribute<NC_RGBM > ncRGBM;

template<>
struct ncAttribute<NC_COLOR> {

    ncAttribute (unsigned int r = 0 , unsigned int g = 0 , unsigned int b = 0 , unsigned int a = 0) : r(r) , g(g) , b(b) , a(a) {};

    ncAttribute operator+(const ncAttribute& color) {
        return ncAttribute(r+color.r,g+color.g,b+color.b,a+color.a);
    }

    ncAttribute& operator+=(const ncAttribute& color) {
        r+=color.r;
        g+=color.g;
        b+=color.b;
        a+=color.a;
        return *this;
    }

    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
};
typedef ncAttribute<NC_COLOR> ncColor;

template<>
struct ncAttribute<NC_PTR> {
    ncAttribute (void* vtree = 0) : value(vtree) {};

    void* value;

};

template <unsigned int N>
struct ncAttributeArray {
    std::vector<ncAttribute<N> > data;
};

struct ncVectorArray : public ncAttributeArray<NC_CHILD> , public ncAttributeArray<NC_CARVE> , public ncAttributeArray<NC_RGBM> , public ncAttributeArray<NC_COLOR> , public ncAttributeArray<NC_PTR> {

    ncVectorArray(unsigned int data_mask = NC_CHILD) : data_mask(data_mask) {
        clear();
    }

    void clear() {
        if (data_mask&NC_CHILD)
            ncAttributeArray<NC_CHILD>::data.clear();
        if (data_mask&NC_CARVE)
             ncAttributeArray<NC_CARVE>::data.clear();
        if (data_mask&NC_RGBM)
             ncAttributeArray<NC_RGBM>::data.clear();
        if (data_mask&NC_COLOR)
             ncAttributeArray<NC_COLOR>::data.clear();
        if (data_mask&NC_PTR)
             ncAttributeArray<NC_PTR>::data.clear();
        size_ = 0;
        newItem();
    }

    void load(std::ifstream& file) {
        unsigned int mask;
        file.read((char *)&mask             , sizeof(unsigned int           ));
        setDataMask(mask);
        unsigned int nritems;
        file.read((char *)&nritems          , sizeof(unsigned int           ));
        resize(nritems);
        if (data_mask&NC_CHILD)
            file.read((char *)&getValue<NC_CHILD>(0)    , nritems*sizeof(ncAttribute<NC_CHILD>));
        if (data_mask&NC_CARVE)
            file.read((char *)&getValue<NC_CARVE>(0)    , nritems*sizeof(ncAttribute<NC_CARVE>));
        if (data_mask&NC_RGBM)
            file.read((char *)&getValue<NC_RGBM>(0)     , nritems*sizeof(ncAttribute<NC_RGBM>));
        if (data_mask&NC_COLOR)
            file.read((char *)&getValue<NC_COLOR>(0)    , nritems*sizeof(ncAttribute<NC_COLOR>));
        if (data_mask&NC_PTR)
            file.read((char *)&getValue<NC_PTR>(0)      , nritems*sizeof(ncAttribute<NC_PTR>));
    }

    void save(std::ofstream& file) {
        file.write((char *)&data_mask       , sizeof(unsigned int           ));
        unsigned int nritems = size();
        file.write((char *)&nritems         , sizeof(unsigned int           ));
        if (data_mask&NC_CHILD)
            file.write((char *)&getValue<NC_CHILD>(0)   , nritems*sizeof(ncAttribute<NC_CHILD>));
        if (data_mask&NC_CARVE)
            file.write((char *)&getValue<NC_CARVE>(0)   , nritems*sizeof(ncAttribute<NC_CARVE>));
        if (data_mask&NC_RGBM)
            file.write((char *)&getValue<NC_RGBM>(0)    , nritems*sizeof(ncAttribute<NC_RGBM>));
        if (data_mask&NC_COLOR)
            file.write((char *)&getValue<NC_COLOR>(0)   , nritems*sizeof(ncAttribute<NC_COLOR>));
        if (data_mask&NC_PTR)
            file.write((char *)&getValue<NC_PTR>(0)     , nritems*sizeof(ncAttribute<NC_PTR>));
    }

    static void copy(ncVectorArray& va_out , unsigned int id_out , ncVectorArray& va_in , unsigned int id_in) {
        unsigned int common_data = va_out.data_mask&va_in.data_mask;

        if (common_data&NC_CHILD)
            va_out.setValue(id_out,va_in.getValue<NC_CHILD>(id_in));
        if (common_data&NC_CARVE)
            va_out.setValue(id_out,va_in.getValue<NC_CARVE>(id_in));
        if (common_data&NC_RGBM)
            va_out.setValue(id_out,va_in.getValue<NC_RGBM>(id_in));
        if (common_data&NC_COLOR)
            va_out.setValue(id_out,va_in.getValue<NC_COLOR>(id_in));
        if (common_data&NC_PTR)
            va_out.setValue(id_out,va_in.getValue<NC_PTR>(id_in));
    }

    unsigned int newItem() {
        if (data_mask&NC_CHILD)
            ncAttributeArray<NC_CHILD>::data.push_back(ncAttribute<NC_CHILD>());
        if (data_mask&NC_CARVE)
            ncAttributeArray<NC_CARVE>::data.push_back(ncAttribute<NC_CARVE>());
        if (data_mask&NC_RGBM)
            ncAttributeArray<NC_RGBM>::data.push_back(ncAttribute<NC_RGBM>());
        if (data_mask&NC_COLOR)
            ncAttributeArray<NC_COLOR>::data.push_back(ncAttribute<NC_COLOR>());
        if (data_mask&NC_PTR)
            ncAttributeArray<NC_PTR>::data.push_back(ncAttribute<NC_PTR>());
        size_++;
        return size_-1;
    }

    unsigned int getItemSize() {
        unsigned int out = 0;
        if (data_mask&NC_CHILD)
            out+=sizeof(ncAttribute<NC_CHILD>);
        if (data_mask&NC_CARVE)
            out+=sizeof(ncAttribute<NC_CARVE>);
        if (data_mask&NC_RGBM)
            out+=sizeof(ncAttribute<NC_RGBM>);
        if (data_mask&NC_COLOR)
            out+=sizeof(ncAttribute<NC_COLOR>);
        if (data_mask&NC_PTR)
            out+=sizeof(ncAttribute<NC_PTR>);
        return out;
    }

    unsigned int size() {
        return size_;
    }

    void setDataMask(unsigned int mask) {
        unsigned int diff = mask^data_mask;
        if (diff&NC_CARVE)
            (diff&mask ? ncAttributeArray<NC_CARVE>::data.resize(size_) : ncAttributeArray<NC_CARVE>::data.clear());
        if (diff&NC_RGBM)
            (diff&mask ? ncAttributeArray<NC_RGBM>::data.resize(size_) : ncAttributeArray<NC_RGBM>::data.clear());
        if (diff&NC_COLOR)
            (diff&mask ? ncAttributeArray<NC_COLOR>::data.resize(size_) : ncAttributeArray<NC_COLOR>::data.clear());
        if (diff&NC_PTR)
            (diff&mask ? ncAttributeArray<NC_PTR>::data.resize(size_) : ncAttributeArray<NC_PTR>::data.clear());

        data_mask = mask;
    }

    void resize(unsigned int new_size) {
        if (data_mask&NC_CHILD)
            ncAttributeArray<NC_CHILD>::data.resize(new_size);
        if (data_mask&NC_CARVE)
            ncAttributeArray<NC_CARVE>::data.resize(new_size);
        if (data_mask&NC_RGBM)
            ncAttributeArray<NC_RGBM>::data.resize(new_size);
        if (data_mask&NC_COLOR)
            ncAttributeArray<NC_COLOR>::data.resize(new_size);
        if (data_mask&NC_PTR)
            ncAttributeArray<NC_PTR>::data.resize(new_size);
        size_ = new_size;
    }

    template<unsigned int N>
    ncAttribute<N>& getValue(unsigned int index) {
        return ncAttributeArray<N>::data[index];
    }

    template<unsigned int N>
    void setValue(unsigned int index , ncAttribute<N> val) {
        ncAttributeArray<N>::data[index] = val;
    }

    template<unsigned int N , typename...T>
    void setValue(unsigned int index , T... val) {
        setValue( index , ncAttribute<N>(val...) );
    }

    unsigned int data_mask;
    unsigned int size_;

};

struct ncVoxTreeIterator {

    ncVoxTreeIterator(unsigned int id_ = 0,ncc::v4i pos_ = ncc::v4i(0,0,0,0)) {
        id  = id_;
        pos = pos_;
    }

    ~ncVoxTreeIterator() {
        id = 0;
        pos = ncc::v4i(0,0,0,0);
    }

    bool operator==(const ncVoxTreeIterator& it) {
        return ((pos == it.pos) && (id == it.id));
    }

    void print(char* txt) {
        if (txt)
            fprintf(stderr,"%s\n",txt);
        pos.print("pos");
        fprintf(stderr,"id = %d\n",id);
    }

    ncc::v4i        pos;
    unsigned int    id;
};


class ncVoxTreeBase {
public:

    void                print();
    void                clear();
    void                load(std::ifstream& file);
    void                save(std::ofstream& file , bool optimize);

    ncc::RTd            getTransform();

    ncVoxTreeIterator   getIterator(ncc::v4i pos);
    void                getIterator(ncVoxTreeIterator & vp , int level);

    virtual void        optimizeChilds();
    unsigned int        split(unsigned int id , int level);
    ncVoxTreeIterator   subDivideTo(ncc::v4i pos);
    void                subDivideTo(ncVoxTreeIterator& vp , int level);

    std::string         filename;
    ncc::v3ll           pmin;
    int                 maxlevel;
    int                 minlevel;
    unsigned int        maxcarve;

    ncVectorArray       voxels;
};


//! ncSubVoxTree
class ncSubVoxTree : public ncVoxTreeBase {
public:
    ncSubVoxTree();
    ncSubVoxTree(std::string fname );
    ncSubVoxTree(std::string fname , unsigned int data_mask);
    ncSubVoxTree(std::string fname , unsigned int data_mask , const ncSubVoxTree& vtree );
    ncSubVoxTree(std::string fname , unsigned int data_mask , ncc::v3ll pmin , int maxlevel , int minlevel , unsigned int maxcarve);
    ~ncSubVoxTree();


    void                setDataMask(unsigned int mask);
    void                incCarve(unsigned int id , int level);
    unsigned int        split(unsigned int id , int level);


    void                clear();
    void                load(std::string fname);
    void                save();
    void                print();

    void                pushID(unsigned int id);
    unsigned int        popID();
    void                clearID(unsigned int id);


    void                optimizeCarve( unsigned int max_carve , int min_level);
    int                 optimizeCarveRecursive(unsigned int id , int cur_level);

    void                subDivide();
    void                subDivide(ncVoxTreeIterator vp);
    void                splitSurface(unsigned int vox , int level , unsigned int locmask);

    void                createColors();
    void                colorUp();
    ncColor             colorUp(unsigned int id,  int level);
    void                colorDown(ncColor color = ncColor(0,0,0,0), unsigned int id = 0);

//protected:
   std::vector<unsigned int>    memory_pool;

};

//! ncVoxTree
class ncVoxTree : public ncVoxTreeBase {
public:
    ncVoxTree();
    ncVoxTree(std::string fname  , bool create_sublevel  , bool save_to_disk  );
    ncVoxTree(std::string fname  , bool create_sublevel  , bool save_to_disk  , unsigned int data_mask);
    ncVoxTree(std::string fname  , bool create_sublevel  , bool save_to_disk  , unsigned int data_mask  , ncc::v3ll pmin , int maxlevel , int sublevel , int minlevel , unsigned int maxcarve);
    ncVoxTree(std::string fname_ , bool create_sublevel_ , bool save_to_disk_ , unsigned int data_mask_ , const ncVoxTree& vtree );
    ~ncVoxTree();

    void                    clear();
    void                    load(std::string fname , bool create_sublevel , bool save_to_disk);
    void                    save();
    void                    print();

    void                    setDataMask(unsigned int mask);
    std::string             getFilename(ncc::v3i vpos);

    void                    loadSubLevel(unsigned int index);
    unsigned int            addSubLevel(ncc::v3i pos);
    void                    saveSubLevel(unsigned int index);

    virtual void            loadSubLevel(ncVoxTreeIterator it);
    virtual void            loadSubLevel(ncVoxTreeIterator it , unsigned int data_mask , ncc::v3ll pmin , int maxlevel , int minlevel , unsigned int maxcarve);
    virtual void            saveSubLevel(ncVoxTreeIterator it);

    static void             copy(std::string fname_out,std::string fname_in,unsigned int mask);

    std::vector<ncc::v3i >  getSubLevelsAtCameraPosition(ncc::v3d campos);
    void                    setCameraPosition(ncc::v3d campos);
    double                  getCameraDistance(ncc::v3d campos,ncc::v4i subl);

    void                    optimizeCarve( unsigned int max_carve , int min_level);
    void                    createColors();
    void                    subDivide();

    /*

    void            addID(unsigned int id_out, unsigned int id_in , ncVectorArray & vin);

    void            split(unsigned int id , int level);




//protected:
    */

    int                             sublevel;
    std::list<unsigned int >        LRU;
    std::vector<ncVoxTreeIterator > subs;
    unsigned int                    max_resident;
    unsigned int                    data_mask;
    bool                            create_sublevel;
    bool                            save_to_disk;
};


#endif //__NC_VOXEL_H__
