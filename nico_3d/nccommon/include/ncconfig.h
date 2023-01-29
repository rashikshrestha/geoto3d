#ifndef __NC_CONFIG_H__
#define __NC_CONFIG_H__

#undef NoError
#include <QDir>
#include <QDomDocument>
#include <QMessageBox>
#include <QFileInfo>
#include "nccalibrationset.h"
#include "ncposelist.h"
#include "nctracklist.h"
#include "ncteximage.h"
#include "nccommon.h"
#include "ncfbo.h"

#define DEFAULT_ZN      2.5
#define NC_BBOX_AUTO    0x0
#define NC_BBOX_MANUAL  0x1

enum file_enum  { XML_FILE , GVM_FILE , GVM_RAW_FILE , GVM_OPT_FILE , GVM_SUBDIV_FILE , GVM_COLOR_FILE};
enum dir_enum   { WORK_DIR , FLOW_DIR , DATA_DIR , MODEL_DIR , MERGED_DIR , VOX_DIR , GVM_DIR , PLY_DIR  , TEST_DIR };
enum cp_enum    { COLOR_CP , DEPTH_CP , DEPTH_CHECK_CP , LAPLACE_CP  , PLY_CP };


#define nr_tv_enum 6
enum tv_enum                                    { TV_3D             , TV_COLOR      , TV_DEPTH      , TV_MASK       , TV_DEPTH_CHECK        , TV_MASK_CHECK     };
const std::string tv_enum_name[nr_tv_enum] =    { "0: TV_3D"        , "1: TV_COLOR" , "2: TV_DEPTH" , "3: TV_MASK"  , "4: TV_DEPTH_CHECK"   , "5: TV_MASK_CHECK"};

template <class T>
struct my_gl_enum {
    GLenum iFormat() { return 0;};
    GLenum format() { return 0;};
    GLenum type() { return 0;};
};

template <>
struct my_gl_enum<ncc::v3ub> {

    GLenum iFormat() { return GL_RGB8;};
    GLenum format() { return GL_RGB;};
    GLenum type() { return GL_UNSIGNED_BYTE;};

};

template <>
struct my_gl_enum<ncc::v4ub> {

    GLenum iFormat() { return GL_RGBA8;};
    GLenum format() { return GL_RGBA;};
    GLenum type() { return GL_UNSIGNED_BYTE;};

};

template <>
struct my_gl_enum<ncc::v2f> {

    GLenum iFormat() { return GL_RG32F;};
    GLenum format() { return GL_RG;};
    GLenum type() { return GL_FLOAT;};
};

namespace GeotoPGFProcessing {
    struct Image
    {
        int W,H,stride;
        unsigned char* data;
        float* hdata;
        float* sdata;
        float* vdata;

        enum type {RGB, HSV};

        Image(): W(0), H(0), data(0), hdata(0), sdata(0), vdata(0)
        {
        }

        Image(int Win, int Hin, int stride, unsigned char* datain): W(Win), H(Hin), stride(stride), hdata(0), sdata(0), vdata(0)
        {
            data = new unsigned char[stride*H];
            memcpy(data, datain, stride*H);
        }

        virtual ~Image()
        {
            if (data)
                delete [] data;
            if (hdata)
                delete [] hdata;
            if (sdata)
                delete [] sdata;
            if (vdata)
                delete [] vdata;

        }

        bool Read(std::string name)
        {
            FILE* fp = fopen(name.c_str(), "rb");
            if (fp == NULL)
            {
                printf("Error (LoadPPM): unable to open %s!\n", name.c_str());
                W = 0;
                H = 0;
                return false;
            }
            char buff[128];

            fgets(buff, 128, fp);
            while(buff[0] == '#' || buff[0] == '\n')    fgets(buff, 128, fp);

            if(strncmp(buff, "P6", 2))
            { fclose(fp); return false; }

            fgets(buff, 128, fp);
            while(buff[0] == '#' || buff[0] == '\n') fgets(buff, 128, fp);


            sscanf(buff, "%d %d", &W, &H);

            if (W==0 || H==0)
            {
                fgets(buff, 128, fp);
                sscanf(buff, "%d %d", &W, &H);
            }

            fgets(buff, 128, fp);
            while(buff[0] == '#') fgets(buff, 128, fp);
            //printf("Reading %dx%d Image [%s]. . .\n", iWidth, iHeight, pFileName);
            int iNumComponents = W * H * 3;
            stride = W*3;
            data = new unsigned char[iNumComponents];
            fread(data, iNumComponents, 1, fp);
            fclose(fp);
            return true;
        }

        unsigned char equalize(const std::vector<int>& cumulative, int v, int min)
        {
            float nv = (cumulative[v] - min)/(float)(W*H-min)*255;
            return ((unsigned char) nv);
        }


        void RGB2HSV()
        {
            if (hdata)
            {
                delete [] hdata; hdata = 0;
            }
            if (sdata)
            {
                delete [] sdata; sdata = 0;
            }
            if (vdata)
            {
                delete [] vdata; vdata = 0;
            }
            if (!hdata)
            {
                hdata = new float[W*H];
                sdata = new float[W*H];
                vdata = new float[W*H];
            }
            for (int i=0; i<W; ++i)
                for (int j=0; j<H; ++j)
                {
                    const float r = data[stride*j+i*3+0]/255.;
                    const float g = data[stride*j+i*3+1]/255.;
                    const float b = data[stride*j+i*3+2]/255.;
                    const float max = (std::max)((std::max)(r,g),b);
                    const float min = (std::min)((std::min)(r,g),b);
                    float h,s,v;
                    if (max == min)
                        h = 0;
                    else
                    {
                        if (max == r)
                            h = (0+(g-b)/(max-min))*60.;
                        if (max == g)
                            h = (2 + (b-r)/(max-min))*60.;
                        if (max == b)
                            h = (4 + (r-g)/(max-min))*60.;
                    }
                    while (h<0)
                        h+=360;
                    while (h>360)
                        h-=360;
                    s = (max-min)/max;
                    v = max;

                    hdata[j*W+i] = h;
                    sdata[j*W+i] = s;
                    vdata[j*W+i] = v;
                }
        }

        void HSV2RGB()
        {
            if (!hdata)
                return;
            for (int i=0; i<W; ++i)
                for (int j=0; j<H; ++j)
                {
                    float r,g,b;
                    const float h = hdata[j*W+i];
                    const float s = sdata[j*W+i];
                    const float v = vdata[j*W+i];
                    if (s == 0)
                        r = g = b = v;
                    else
                    {
                        const int hi = (int) h/60;
                        const float f = h/60.-hi;
                        const float p = v*(1-s);
                        const float q = v*(1-f*s);
                        const float t = v*(1-(1-f)*s);

                        switch(hi)
                        {
                        case 0:
                            r = v;
                            g = t;
                            b = p;
                            break;
                        case 1:
                            r = q;
                            g = v;
                            b = p;
                            break;
                        case 2:
                            r = p;
                            g = v;
                            b = t;
                            break;
                        case 3:
                            r = p;
                            g = q;
                            b = v;
                            break;
                        case 4:
                            r = t;
                            g = p;
                            b = v;
                            break;
                        case 5:
                            r = v;
                            g = p;
                            b = q;
                            break;
                        default:
                            std::cerr << "Hi " << hi << " from h " << h << " should never get here!" << std::endl;
                        }
                    }
                    data[stride*j+i*3+0] = (unsigned char) 255*r;
                    data[stride*j+i*3+1] = (unsigned char) 255*g;
                    data[stride*j+i*3+2] = (unsigned char) 255*b;
                }
        }


        void Equalize(const type t)
        {
            std::vector<int> histogram_r(256,0), histogram_g(256,0), histogram_b(256,0);
            std::vector<int> cum_histogram_r(256,0), cum_histogram_g(256,0), cum_histogram_b(256,0);
            std::vector<int> histogram_v(256, 0);
            std::vector<int> cum_histogram_v(256, 0);

            int min_r(-1), min_g(-1), min_b(-1), min_v(-1);
            if (t == HSV)
                RGB2HSV();

            switch (t)
            {
            case RGB:
            for (int i=0; i<W; ++i)
                for (int j=0; j<H; ++j)
                {
                            histogram_r[data[stride*j+i*3+0/*(j*W+i)*3+0*/]]++;
                            histogram_g[data[stride*j+i*3+1/*(j*W+i)*3+1*/]]++;
                            histogram_b[data[stride*j+i*3+2/*(j*W+i)*3+2*/]]++;
                }
            break;
            case HSV:
            for (int i=0; i<W; ++i)
                for (int j=0; j<H; ++j)
                    histogram_v[(unsigned int)(vdata[j*W+i]*255)]++;
            break;
            }

            switch (t)
            {
            case RGB:
            for (int i=0; i<256; i++)
            {
                if (i==0)
                {
                    cum_histogram_r[i] = histogram_r[i];
                    cum_histogram_g[i] = histogram_g[i];
                    cum_histogram_b[i] = histogram_b[i];
                }
                else
                {
                        cum_histogram_r[i] = cum_histogram_r[i-1] + histogram_r[i];
                        cum_histogram_g[i] = cum_histogram_g[i-1] + histogram_g[i];
                        cum_histogram_b[i] = cum_histogram_b[i-1] + histogram_b[i];
                        if (min_r == -1 && cum_histogram_r[i] != 0)
                            min_r = i;
                        if (min_g == -1 && cum_histogram_g[i] != 0)
                            min_g = i;
                        if (min_b == -1 && cum_histogram_b[i] != 0)
                            min_b = i;
                }
            }
            break;
            case HSV:
            for (int i=0; i<256; i++)
            {
                if (i==0)
                {
                    cum_histogram_v[i] = histogram_v[i];
                }
                else
                {
                        cum_histogram_v[i] = cum_histogram_v[i-1] + histogram_v[i];
                        if (min_v == -1 && cum_histogram_v[i] != 0)
                            min_v = i;
                }
            }
            break;
            }


            const double scale_r = 1/(float)(W*H-min_r)*255.;
            const double scale_g = 1/(float)(W*H-min_g)*255.;
            const double scale_b = 1/(float)(W*H-min_b)*255.;
            const double scale_v = 1/(float)(W*H-min_v)*255.;
            switch (t)
            {
            case RGB:
                for (int i=0; i<W; ++i)
                    for (int j=0; j<H; ++j)
                    {
                        data[stride*j+i*3+0/*(j*W+i)*3+0*/] = (unsigned char)((cum_histogram_r[data[stride*j+i*3+0/*(j*W+i)*3+0*/]]-min_r)*scale_r);
                        data[stride*j+i*3+1/*(j*W+i)*3+1*/] = (unsigned char)((cum_histogram_g[data[stride*j+i*3+1/*(j*W+i)*3+1*/]]-min_g)*scale_g);
                        data[stride*j+i*3+2/*(j*W+i)*3+2*/] = (unsigned char)((cum_histogram_b[data[stride*j+i*3+2/*(j*W+i)*3+2*/]]-min_b)*scale_b);
                    }
            break;
            case HSV:
                for (int i=0; i<W; ++i)
                    for (int j=0; j<H; ++j)
                    {
                        vdata[j*W+i] = ((unsigned char)((cum_histogram_v[(unsigned int)(vdata[j*W+i]*255)]-min_v)*scale_v))/255.;
                    }
                HSV2RGB();
                break;
            }
        }


        bool Write(std::string name)
        {
            FILE* fp = fopen(name.c_str(), "wb");
            if (fp == NULL)
            {
                printf("Error (WritePPM) : unable to open %s!\n", name.c_str());
                return false;
            }
            fprintf(fp, "P6\n%d %d\n255\n", W, H);
            fwrite(data, 1, W * H * 3, fp);
            fclose(fp);
            return true;
        }

    };
}

struct ncTexInfo {

    ncTexInfo()  {}
    ncTexInfo( int cid , int vid , int lod , ncCalibration calib) : cid(cid),vid(vid),lod(lod),calib(calib)  {}

    ncc::m4x4d KGL() {
        return calib.KGL(lod);
    }

    ncc::m4x4d Ktex() {
        return calib.Ktex(lod);
    }

    ncc::RTd MV() {
        return calib.MV();
    }

    int width() {
    	return calib.width(lod);
    }

    int height() {
    	return calib.height(lod);
    }

    void write(std::ofstream& file) {
        file.write((char*)&cid      , sizeof(int         	));
        file.write((char*)&vid      , sizeof(int         	));
        file.write((char*)&lod      , sizeof(int         	));
        file.write((char*)&calib    , sizeof(ncCalibration	));
    }

    void read(std::ifstream& file) {
        file.read((char*)&cid       , sizeof(int         	));
        file.read((char*)&vid       , sizeof(int         	));
        file.read((char*)&lod       , sizeof(int         	));
        file.read((char*)&calib     , sizeof(ncCalibration	));
    }

    int             cid;
    int             vid;
    int             lod;
    ncCalibration	calib;
};

struct ncCommand {

    ncCommand(int cid_, int vid_, int index_, int ref_) : cid(cid_) , vid(vid_) , index(index_) , ref(ref_) {};

    void print() {
        fprintf(stderr,"cid = %d , vid = %d , index = %d , ref = %d\n",cid,vid,index,ref);
    }

    int cid;
    int vid;
    int index;
    int ref;
};

struct ncID {

    ncID(unsigned int cid_,unsigned int vid_,unsigned int status_ = 0) : cid(cid_),vid(vid_) {};
    void print() { fprintf(stderr,"id = %d %d\n",cid,vid);};
    unsigned int cid;
    unsigned int vid;
};

struct ncIDList {

    ncIDList() {
        clear();
    }

    void push_back(ncID id) {
        ids.push_back(id);
    };

    void clear() {
        ids.clear();
    }

    ncID& operator[] (int index) {

        int newindex = index;
        while (newindex<0)
            newindex+=ids.size()-1;

        int a = newindex%(ids.size()-1);
        int b = newindex/(ids.size()-1);
        if ((index<0)^(b%2))
            return ids[ids.size()-1-a];
        else
            return ids[a];
    }

    ncID& back() {
            return ids.back();
    }

    unsigned int size() { return ids.size();};

    void print() {
        fprintf(stderr,"IDList:\n");
        for (unsigned int i=0;i<ids.size();++i)
            fprintf(stderr,"%06d : %02d %06d\n",i,ids[i].cid,ids[i].vid);
    }

    std::vector<ncID> ids;
};

static void insertTabs(FILE* ofile , int level) {
    for (int i=0;i<level;++i)
        fprintf(ofile,"\t");
}

struct ncConfigProjectCameras {

     ncConfigProjectCameras() : calset(0) {
        clear();
    };

    ~ncConfigProjectCameras() { clear(); };

    void clear() {
        filename.clear();
        directory.clear();
        if (calset)
        {
            delete calset;
            calset = 0;
        }

        for (unsigned int i=0;i<MAX_CAM;++i)
        {
            znear[i]    = 1.0;
            zfar[i]     = 20.0;
        	active[i]	= false;
        }
    }

    void print() {
        fprintf(stderr,"directory : %s\n",directory.c_str());
        fprintf(stderr,"filename  : %s\n",filename.c_str());
        double zn,zf;
        for (unsigned int i=0;i<calset->getNrCameras();++i)
            fprintf(stderr,"cid[%2d] active[%d] zn[%8.7f] zf[%8.7f] vignet[%8.7f] gamma[%8.7f] scale[%8.7f]\n",i,active[i],znear[i],zfar[i],(*calset)[i].getVignet(),(*calset)[i].getGamma(),(*calset)[i].getScale());
    }

    bool parse(const QDomElement &element) {
        QDomElement child;

        std::string tmpfile;

        child = element.firstChildElement("file");
        if (!child.isNull())
        {
            tmpfile     = child.text().trimmed().toStdString();
            directory   = ncc::getDirName(tmpfile);
            filename    = ncc::getFileName(tmpfile);
        }
        else
        {
            child = element.firstChildElement("filename");
            if (!child.isNull())
                filename     = child.text().trimmed().toStdString();

            child = element.firstChildElement("directory");
            if (!child.isNull())
                directory     = child.text().trimmed().toStdString();
        }

        if (!ncc::checkFile(getPath()))
        {
            fprintf(stderr,"Error: Could not open \"%s\"  in (%s %d)\n",getPath().c_str(),__FILE__,__LINE__);
        	return false;
		}
        else
            calset = new ncCalibrationSet(getPath(),DEFAULT_ZN);

        child = element.firstChildElement("settings");
        while (!child.isNull()) {

            if (child.hasAttribute("cid"))
            {
                unsigned int    cid     = child.attribute("cid").toInt();
                if (cid<calset->getNrCameras())
                {
                    if (child.hasAttribute("active"))
                        active[cid] = child.attribute("active").toInt();
                    if (child.hasAttribute("znear"))
                    {
                        znear[cid]   = child.attribute("znear").toDouble();
                        (*calset)(cid).setDepthRange(znear[cid]);
                    }
                    if (child.hasAttribute("zfar"))
                        zfar[cid]       = child.attribute("zfar").toDouble();
                    if (child.hasAttribute("vignet"))
                        (*calset)(cid).setVignet( child.attribute("vignet").toDouble() );
                    if (child.hasAttribute("gamma"))
                        (*calset)(cid).setGamma( child.attribute("gamma").toDouble() );
                    if (child.hasAttribute("scale"))
                        (*calset)(cid).setScale( child.attribute("scale").toDouble() );
                    if (child.hasAttribute("wb"))
                    {
                		QString 	wb 	= child.attribute("wb");
                		QStringList wbl	= wb.split(':');
                        (*calset)(cid).setWB( ncc::v3f(wbl[0].toDouble(),wbl[1].toDouble(),wbl[2].toDouble()) );
					}
				}
            }
            child = child.nextSiblingElement("settings");
        }
        return true;
    }

    void save(FILE* ofile , int level) {

        insertTabs(ofile,level);
        fprintf(ofile,"<cameras>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<directory> %s </directory>\n",directory.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<filename> %s </filename>\n",filename.c_str());
        for (unsigned int i=0;i<calset->getNrCameras();++i)
        {
            insertTabs(ofile,level+1);
            const ncCalibration& cam = (*calset)[i];
            fprintf(ofile,"<settings cid=\"%d\" active=\"%d\" znear=\"%f\" zfar=\"%f\" vignet=\"%f\" gamma=\"%f\" scale=\"%f\" wb=\"%f:%f:%f\" />\n", i, active[i] , znear[i], zfar[i], cam.getVignet(),cam.getGamma(),cam.getScale(),cam.getWB().x,cam.getWB().y,cam.getWB().z);
        }
        insertTabs(ofile,level);
        fprintf(ofile,"</cameras>\n");
    }

    bool getMaskName(char* mname , int cid) {
        sprintf(mname,"%s/mask.%d.pgm",directory.c_str(),cid);
        return ncc::checkFile(mname);
    }

    std::string getPath() {
        return directory + "/" + filename;
    }

    std::string                 filename;
    std::string                 directory;
    ncCalibrationSet*           calset;
    double                      znear[MAX_CAM];
    double                      zfar[MAX_CAM];
    bool                      	active[MAX_CAM];

};

struct ncConfigProjectPoses {

     ncConfigProjectPoses() : poselist(0) {
	     clear();
    };

    ~ncConfigProjectPoses() { clear(); };

    void clear() {
        directory.clear();
        filename.clear();
        if (poselist)
        {
            delete poselist;
            poselist = 0;
        }
        start_pose 	= 0;
        end_pose	= 0;
        step_pose	= 0.3;
    }

    void print() {
        fprintf(stderr,"poselist    : %s\n",getPath().c_str());
        fprintf(stderr,"start_pose  : %d\n",start_pose);
        fprintf(stderr,"end_pose    : %d\n",end_pose);
        fprintf(stderr,"step_pose   : %f\n",step_pose);
    }

    bool parse(const QDomElement &element) {
        QDomElement child;

        child = element.firstChildElement("file");
        if (!child.isNull())
        {
            directory = ncc::getDirName(child.text().trimmed().toStdString());
            filename  = ncc::getFileName(child.text().trimmed().toStdString());
        }
        else
        {
            child = element.firstChildElement("directory");
            if (!child.isNull())
                directory = child.text().trimmed().toStdString();

            child = element.firstChildElement("filename");
            if (!child.isNull())
                filename = child.text().trimmed().toStdString();
        }

        child = element.firstChildElement("start_pose");
        if (!child.isNull())
            start_pose = child.text().trimmed().toInt();

        child = element.firstChildElement("end_pose");
        if (!child.isNull())
            end_pose = child.text().trimmed().toInt();

        child = element.firstChildElement("step_pose");
        if (!child.isNull())
            step_pose = child.text().trimmed().toDouble();

        if (!ncc::checkFile(getPath()))
        {
            fprintf(stderr,"Error: Could not open \"%s\"  in (%s %d)\n",getPath().c_str(),__FILE__,__LINE__);
        	return false;
		}
        else
            poselist = new ncPoseList(getPath());

    	return true;
    }

    void save(FILE* ofile , int level) {

        insertTabs(ofile,level);
        fprintf(ofile,"<poses>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<directory> %s </directory>\n",directory.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<filename> %s </filename>\n",filename.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<start_pose> %d </start_pose>\n",start_pose);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<end_pose> %d </end_pose>\n",end_pose);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<step_pose> %f </step_pose>\n",step_pose);
        insertTabs(ofile,level);
        fprintf(ofile,"</poses>\n");
    }

    std::string getPath() {
        return directory + "/" + filename;
    }


    std::string   directory;
    std::string   filename;
    ncPoseList*   poselist;
    int           start_pose;
    int           end_pose;
    double        step_pose;

};

struct ncConfigProjectImages {

     ncConfigProjectImages() { clear();  };

    ~ncConfigProjectImages() { clear(); };

    void clear() {
        im_info = ImageInfo();
    };

    void print() {
        im_info.print();
    }

    bool parse(const QDomElement &element , ncCalibrationSet& calset , ncPoseList& poselist) {
        QDomElement child;

        child = element.firstChildElement("directory");
        if (!child.isNull())
            im_info.setup(child.text().trimmed().toStdString() , calset , poselist);

		return true;
    }

    void save(FILE* ofile , int level) {
        insertTabs(ofile,level);
        fprintf(ofile,"<images>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<directory> %s </directory>\n",im_info.directory.c_str());
        insertTabs(ofile,level);
        fprintf(ofile,"</images>\n");
    }


    ImageInfo im_info;
};

struct ncConfigProjectTracks {

     ncConfigProjectTracks() : tracklist(0) { clear();  };

    ~ncConfigProjectTracks() { clear(); };

    void clear() {
        if (tracklist)
        {
            delete tracklist;
            tracklist = 0;
        }
        directory.clear();
        regexp.clear();
    };

    void print() {
        fprintf(stderr,"tracks  : %s\n",(directory+regexp).c_str());
    }

    bool parse(const QDomElement &element) {
        QDomElement child;

        child = element.firstChildElement("directory");
        if (!child.isNull())
            directory = child.text().trimmed().toStdString();

        child = element.firstChildElement("regexp");
        if (!child.isNull())
            regexp = child.text().trimmed().toStdString();

		return true;
    }

    void save(FILE* ofile , int level) {
        insertTabs(ofile,level);
        fprintf(ofile,"<tracks>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<directory> %s </directory>\n",directory.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<regexp> %s </regexp>\n",regexp.c_str());
        insertTabs(ofile,level);
        fprintf(ofile,"</tracks>\n");
    }

    bool fromDirName(std::string fname) {

        directory    = fname;
        regexp       = "*.track";

        return true;
    }

    ncTrackList*    tracklist;
    std::string     directory;
    std::string     regexp;

};

struct ncConfigProject {

    ncConfigProject() { clear(); };

    ~ncConfigProject() { clear(); };

    void clear() {
        cameras.clear();
        poses.clear();
        images.clear();
        tracks.clear();
    }

    void print() {
        cameras.print();
        poses.print();
        images.print();
        tracks.print();
    }

    bool parse(const QDomElement &element) {
        QDomElement child;

        bool valid = true;

        child = element.firstChildElement("cameras");
        if (!child.isNull())
            valid = valid && cameras.parse(child);

        child = element.firstChildElement("poses");
        if (!child.isNull())
            valid = valid && poses.parse(child);

        child = element.firstChildElement("images");
        if (!child.isNull())
            valid = valid && images.parse(child,*cameras.calset,*poses.poselist);

        child = element.firstChildElement("tracks");
        if (!child.isNull())
            valid = valid && tracks.parse(child);

		return valid;
    }

    void save(FILE* ofile , int level) {

        insertTabs(ofile,level);
        fprintf(ofile,"<project>\n");

        cameras.save(ofile,level+1);
        poses.save(ofile,level+1);
        images.save(ofile,level+1);
        tracks.save(ofile,level+1);

        insertTabs(ofile,level);
        fprintf(ofile,"</project>\n");
    }

    ncConfigProjectCameras cameras;
    ncConfigProjectPoses   poses;
    ncConfigProjectImages  images;
    ncConfigProjectTracks  tracks;
};

struct ncConfigFlow {

    ncConfigFlow() {
        clear();
    };

    ~ncConfigFlow() { clear();};

    void clear() {
        imrange     = 3;
        winlod      = 3;
        imlod       = 2;
        winD        = 512;
        linP        = 0.8f;
        gauss       = 2.0f;
    }

    void print() {

        fprintf(stderr,"imrange     : %d\n",imrange);
        fprintf(stderr,"imlod       : %d\n",imlod);
        fprintf(stderr,"winlod      : %d\n",winlod);
        fprintf(stderr,"winD        : %d\n",winD);
        fprintf(stderr,"linP        : %f\n",linP);
        fprintf(stderr,"gauss       : %f\n",gauss);
        fprintf(stderr,"\n");
    }

    bool parse(const QDomElement &element) {

        QDomElement child;

        child = element.firstChildElement("imrange");
        if (!child.isNull())
            imrange = child.text().trimmed().toInt();

        child = element.firstChildElement("winlod");
        if (!child.isNull())
            winlod = child.text().trimmed().toInt();

        child = element.firstChildElement("imlod");
        if (!child.isNull())
            imlod = child.text().trimmed().toInt();

        child = element.firstChildElement("winD");
        if (!child.isNull())
            winD = child.text().trimmed().toInt();

        child = element.firstChildElement("linP");
        if (!child.isNull())
            linP = child.text().trimmed().toFloat();

        child = element.firstChildElement("gauss");
        if (!child.isNull())
            gauss = child.text().trimmed().toFloat();

		return true;
    }

    void save(FILE* ofile , int level)     {
        insertTabs(ofile,level);
        fprintf(ofile,"<flow>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<imrange> %d </imrange>\n",imrange);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<imlod> %d </imlod>\n",imlod);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<winlod> %d </winlod>\n",winlod);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<winD> %d </winD>\n",winD);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<linP> %f </linP>\n",linP);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<gauss> %f </gauss>\n",gauss);
        insertTabs(ofile,level);
        fprintf(ofile,"</flow>\n");
    }

    int                 imrange;
    int                 winlod;
    int                 imlod;
    int                 winD;
    float               linP;
    float               gauss;
};

struct ncConfigVox {

    ncConfigVox() {
        clear();
    };

    ~ncConfigVox() { clear();};

    void clear() {
        max_carve   = 16;
        min_level   = 6;
        bb_min      = ncc::v3d(0.0,0.0,0.0);
        bb_max      = ncc::v3d(0.0,0.0,0.0);
        bb_mode     = NC_BBOX_AUTO;
        upscale     = 0;
    }

    void print() {

        fprintf(stderr,"upscale     : %d\n",upscale);
        fprintf(stderr,"max_carve   : %d\n",max_carve);
        fprintf(stderr,"min_level   : %d\n",min_level);
        fprintf(stderr,"bb_min      : %+10.5f %+10.5f %+10.5f\n",bb_min.x,bb_min.y,bb_min.z);
        fprintf(stderr,"bb_max      : %+10.5f %+10.5f %+10.5f\n",bb_max.x,bb_max.y,bb_max.z);
    }

    bool parse(const QDomElement &element) {

        QDomElement child;

        child = element.firstChildElement("upscale");
        if (!child.isNull())
            upscale = child.text().trimmed().toInt();

        child = element.firstChildElement("max_carve");
        if (!child.isNull())
            max_carve = child.text().trimmed().toInt();

        child = element.firstChildElement("min_level");
        if (!child.isNull())
            min_level = child.text().trimmed().toInt();

        bb_mode  = NC_BBOX_AUTO;
        child = element.firstChildElement("bbox");
        if (!child.isNull())
        {
            QString mode = child.attribute("mode");
            if (mode.toLower() == "auto")
                bb_mode  = NC_BBOX_AUTO;
            else if (mode.toLower() == "manual")
                bb_mode  = NC_BBOX_MANUAL;
            else
                fprintf(stderr,"Error while processing bounding box. No mode defined...\n");

            QStringList bbox_elem = child.text().split(" ", QString::SkipEmptyParts);
            if (bbox_elem.size()==6)
            {
                bb_min.x = bbox_elem[0].toDouble();
                bb_min.y = bbox_elem[1].toDouble();
                bb_min.z = bbox_elem[2].toDouble();
                bb_max.x = bbox_elem[3].toDouble();
                bb_max.y = bbox_elem[4].toDouble();
                bb_max.z = bbox_elem[5].toDouble();
            }
        }

        return true;
    }

    void save(FILE* ofile , int level)
    {
        insertTabs(ofile,level);
        fprintf(ofile,"<vox>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<upscale> %d </upscale>\n",upscale);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<max_carve> %d </max_carve>\n",max_carve);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<min_level> %d </min_level>\n",min_level);
        insertTabs(ofile,level+1);
        if (bb_mode == NC_BBOX_MANUAL)
            fprintf(ofile,"<bbox mode=\"manual\"> %f %f %f %f %f %f </bbox>\n",bb_min.x,bb_min.y,bb_min.z,bb_max.x,bb_max.y,bb_max.z);
        else if (bb_mode == NC_BBOX_AUTO)
            fprintf(ofile,"<bbox mode=\"auto\"> %f %f %f %f %f %f </bbox>\n",bb_min.x,bb_min.y,bb_min.z,bb_max.x,bb_max.y,bb_max.z);
        insertTabs(ofile,level);
        fprintf(ofile,"</vox>\n");
    }

    int             max_carve;
    int             min_level;
    ncc::v3d        bb_min;
    ncc::v3d        bb_max;
    unsigned int    bb_mode;
    int             upscale;
};

struct ncConfigOrtho {

    ncConfigOrtho() {
        clear();
    };

    ~ncConfigOrtho() { clear();};

    void clear() {
    	zmin		= 2.4;
    	zmax		= 3.0;
    	zrad		= 15.0;
        stroke		= 12.0;
        grayscale	= false;
        extra_tracks_dir.clear();
        extra_points_dir.clear();
        output_dir.clear();
    }

    void print() {

        fprintf(stderr,"zmin  		: %f\n",zmin);
        fprintf(stderr,"zmax  		: %f\n",zmax);
        fprintf(stderr,"zrad  		: %f\n",zrad);
        fprintf(stderr,"stroke  	: %f\n",stroke);
        fprintf(stderr,"grayscale   : %d\n",grayscale);
        fprintf(stderr,"extra_tracks: %s\n",extra_tracks_dir.c_str());
        fprintf(stderr,"extra_points: %s\n",extra_points_dir.c_str());
        fprintf(stderr,"output   	: %s\n",output_dir.c_str());
    }

    bool parse(const QDomElement &element) {

        QDomElement child;

        child = element.firstChildElement("stroke");
        if (!child.isNull())
            stroke = child.text().trimmed().toDouble();

        child = element.firstChildElement("grayscale");
        if (!child.isNull())
            grayscale = child.text().trimmed().toInt();

        child = element.firstChildElement("extra_tracks");
        if (!child.isNull())
            extra_tracks_dir = child.text().trimmed().toStdString();

        child = element.firstChildElement("extra_points");
        if (!child.isNull())
            extra_points_dir = child.text().trimmed().toStdString();

        child = element.firstChildElement("output");
        if (!child.isNull())
            output_dir = child.text().trimmed().toStdString();

        child = element.firstChildElement("zmin");
        if (!child.isNull())
            zmin = child.text().trimmed().toDouble();

        child = element.firstChildElement("zmax");
        if (!child.isNull())
            zmax = child.text().trimmed().toDouble();

        child = element.firstChildElement("zrad");
        if (!child.isNull())
            zrad = child.text().trimmed().toDouble();

		return true;
    }

    void save(FILE* ofile , int level) {
        insertTabs(ofile,level);
        fprintf(ofile,"<ortho>\n");
        insertTabs(ofile,level+1);
        fprintf(ofile,"<zmin> %f </zmin>\n",zmin);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<zmax> %f </zmax>\n",zmax);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<zrad> %f </zrad>\n",zrad);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<stroke> %f </stroke>\n",stroke);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<grayscale> %d </grayscale>\n",grayscale);
        insertTabs(ofile,level+1);
        fprintf(ofile,"<extra_tracks> %s </extra_tracks>\n",extra_tracks_dir.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<extra_points> %s </extra_points>\n",extra_points_dir.c_str());
        insertTabs(ofile,level+1);
        fprintf(ofile,"<output> %s </output>\n",output_dir.c_str());
        insertTabs(ofile,level);
        fprintf(ofile,"</ortho>\n");
    }

    double			zmin;
    double			zmax;
    double			zrad;
    double          stroke;
    bool			grayscale;
    std::string		extra_tracks_dir;
    std::string		extra_points_dir;
    std::string		output_dir;
};

class ncConfig {

public:

    ncConfig() {
        ncConfig::clear();
    };

    ~ncConfig() { clear(); };

    ncConfig(std::string fname) {
        clear();
        load(fname);
    }

    std::string getFileName(file_enum file) {
        char name[1024];
        switch (file) {
            case XML_FILE:
                sprintf(name , "%s/config.xml",getDirName(FLOW_DIR).c_str());
                break;
            case GVM_FILE:
                sprintf(name , "%s/test.gvm",getDirName(GVM_DIR).c_str());
                break;
            case GVM_RAW_FILE:
                sprintf(name , "%s/test.raw.gvm",getDirName(GVM_DIR).c_str());
                break;
            case GVM_OPT_FILE:
                sprintf(name , "%s/test.opt.gvm",getDirName(GVM_DIR).c_str());
                break;
            case GVM_SUBDIV_FILE:
                sprintf(name , "%s/test.subdiv.gvm",getDirName(GVM_DIR).c_str());
                break;
            case GVM_COLOR_FILE:
                sprintf(name , "%s/test.color.gvm",getDirName(GVM_DIR).c_str());
                break;
            default:
                fprintf(stderr,"Error: Unknown file enumerator in config\n");
                break;
        }
        return std::string(name);
    }

    std::string getFileName(cp_enum cp , unsigned int cid , unsigned int pid) {
        char name[1024];
        switch (cp) {
            case DEPTH_CP:
                sprintf(name , "%s/%02d.%06d.depth",getDirName(DATA_DIR).c_str(),cid,pid);
                break;
            case DEPTH_CHECK_CP:
                sprintf(name , "%s/%02d.%06d.check",getDirName(DATA_DIR).c_str(),cid,pid);
                break;
            case COLOR_CP:
                sprintf(name , "%s/%02d.%06d.color",getDirName(DATA_DIR).c_str(),cid,pid);
                break;
            case LAPLACE_CP:
                sprintf(name , "%s/%02d.%06d.laplace",getDirName(DATA_DIR).c_str(),cid,pid);
                break;
            //case PLY_CP:
            //    sprintf(name , "%s/out.%d.%%d.%%d.ply",getDirName(PLY_DIR).c_str(),cid,pid);
            //    break;
            default:
                fprintf(stderr,"Error: Unknown file enumerator in config\n");
                break;
        }
        return std::string(name);
    }

	std::string getImageFileName(unsigned int cid , unsigned int pid) {
        return project.images.im_info.getFileName(cid,pid);
	}

	std::string getDirName(dir_enum dir) {
        char name[1024];
        switch (dir) {
            case WORK_DIR:
                sprintf(name , "%s",work_dir.c_str());
                break;
            case FLOW_DIR:
                sprintf(name , "%s/GeoToFlow",getDirName(WORK_DIR).c_str());
                break;
            case DATA_DIR:
                sprintf(name , "%s/data",getDirName(FLOW_DIR).c_str());
                break;
            case MODEL_DIR:
                sprintf(name , "%s/mdl",getDirName(FLOW_DIR).c_str());
                break;
            case MERGED_DIR:
                sprintf(name , "%s/merged",getDirName(FLOW_DIR).c_str());
                break;
            case VOX_DIR:
                sprintf(name , "%s/vox",getDirName(FLOW_DIR).c_str());
                break;
            case GVM_DIR:
                sprintf(name , "%s/gvm",getDirName(FLOW_DIR).c_str());
                break;
            case PLY_DIR:
                sprintf(name , "%s/ply",getDirName(FLOW_DIR).c_str());
                break;
            case TEST_DIR:
                sprintf(name , "%s/test",getDirName(FLOW_DIR).c_str());
                break;
            default:
                fprintf(stderr,"Error: Unknown directory enumerator in config\n");
                break;
        }
        return std::string(name);
    }

    void clobber() {

        char syscall[1024];

        sprintf(syscall,"rm -rf %s",getDirName(FLOW_DIR).c_str());
        system(syscall);
        sprintf(syscall,"rm -rf %s",getDirName(VOX_DIR).c_str());
        system(syscall);
    }

    void clear() {

		valid = false;
        file_name.resize(1024);
        file_name.clear();
        work_dir.resize(1024);
        sprintf(&work_dir[0] , "/usr/data/tmp/%s",getenv("USER"));
        my_tv_enum = TV_3D;
        rectify = false;

        project.clear();
        ortho.clear();
        flow.clear();
        vox.clear();
    }

    virtual bool load(std::string fname) {

        clear();

        QDomDocument domDocument;
        QString errorStr;
        int errorLine;
        int errorColumn;

        QFile file(fname.c_str());
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::information(0, QString("GeoTo3D"), QString("Cannot read file %1:\n%2.").arg(QString(fname.c_str())).arg(file.errorString()));
            return false;
        }

        if (!domDocument.setContent((QIODevice*)&file, true, &errorStr, &errorLine, &errorColumn)) {
            QMessageBox::information(0, QString("GeoTo3D"), QString("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
            return false;
        }

        QDomElement root = domDocument.documentElement();
        if (root.tagName() != "GeoTo") {
            QMessageBox::information(0, QString("GeoTo"), QString("The file is not a GeoTo file."));
            return false;
        } else if (root.hasAttribute("version") && root.attribute("version") != "1.0") {
            QMessageBox::information(0, QString("GeoTo"), QString("The file is not a GeoTo version 1.0 file."));
            return false;
        }

        QDomElement child;

        valid = true;

        child = root.firstChildElement("project");
        while (valid && !child.isNull()) {
            valid = valid && project.parse(child);
            child = child.nextSiblingElement("project");
        }

        child = root.firstChildElement("ortho");
        while (valid && !child.isNull()) {
            valid = valid && ortho.parse(child);
            child = child.nextSiblingElement("ortho");
        }

        child = root.firstChildElement("flow");
        while (valid && !child.isNull()) {
            valid = valid && flow.parse(child);
            child = child.nextSiblingElement("flow");
        }

        child = root.firstChildElement("vox");
        while (valid && !child.isNull()) {
            valid = valid && vox.parse(child);
            child = child.nextSiblingElement("vox");
        }

        file_name = fname;

		ncc::createDir(getDirName(FLOW_DIR));

        if (!valid)
        	clear();

		return valid;
    }

    void apply() {
		if (valid)
			save(file_name);
    }

    virtual void save(std::string fname) {

		ncc::createDir(QFileInfo(fname.c_str()).path().toStdString());
        FILE* ofile = fopen(fname.c_str(),"w");
        save(ofile,0);
        fclose(ofile);

    }

    void save(FILE* ofile , int level) {
        insertTabs(ofile,level);
        fprintf(ofile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        insertTabs(ofile,level);
        fprintf(ofile,"<!DOCTYPE GeoTo>\n");
        insertTabs(ofile,level);
        fprintf(ofile,"<GeoTo version=\"1.0\">\n");

        project.save(ofile,level+1);
        ortho.save(ofile,level+1);
        flow.save(ofile,level+1);
        vox.save(ofile,level+1);

        insertTabs(ofile,level);
        fprintf(ofile,"</GeoTo>\n");

    }

    void print() {
        project.print();
        ortho.print();
        flow.print();
        vox.print();
    }

    std::vector<int> createIDList() {

        fprintf(stderr,"creating id list...\n");

        std::vector<int> valid_list;
        for (int pid = project.poses.start_pose; pid<=project.poses.end_pose;pid++)
	        if (project.poses.poselist->isValid(pid))
	        	valid_list.push_back(pid);

        ncc::v3d 	last_van_pos,van_pos1,van_pos2;
        std::vector<int> id_list;

		if (valid_list.size()>0)
		{
			int i1			= 0;
			int pid1 		= valid_list[i1];

			last_van_pos	= (*project.poses.poselist)[pid1][3].xyz();
			id_list.push_back(pid1);
			i1++;

			while (i1 < valid_list.size())
			{
				pid1 			= valid_list[i1];
				van_pos1 		= (*project.poses.poselist)[pid1][3].xyz();
				double dist1	= fabs( project.poses.step_pose - (van_pos1-last_van_pos).norm() );

				int i2 = i1+1;
				while (i2 < valid_list.size())
				{
					int pid2 		= valid_list[i2];
					van_pos2 		= (*project.poses.poselist)[pid2][3].xyz();
					double dist2	= fabs( project.poses.step_pose - (van_pos2-last_van_pos).norm() );
					if (dist2<dist1)
					{
						i1 		= i2;
						dist1 	= dist2;
						i2++;
					}
					else
						break;
				}

				pid1 			= valid_list[i1];
				last_van_pos	= (*project.poses.poselist)[pid1][3].xyz();
				id_list.push_back(pid1);
				i1++;
			}
		}

		//for (int i=0;i<id_list.size(); ++i)
		//	fprintf(stderr,"id[%d] = %d\n",i,id_list[i]);

		return id_list;
    }

    ncIDList createIDList(int cid) {

        fprintf(stderr,"creating id list for camera %d...\n",cid);

        std::vector<int> 	plist = createIDList();
        ncIDList 			id_list;

		for (unsigned int i=0;i<plist.size();++i)
			if (ncc::checkFile(getImageFileName(cid,plist[i])))
				id_list.push_back(ncID(cid,plist[i]));

        return id_list;
    }

    std::vector<ncCommand> createCMDList(int cid) {

        ncIDList id_list = createIDList(cid);

        int nrim        = 2*flow.imrange+1;
        int ld__        = 0;
        int ref__       = 0;

        if (id_list.size() < nrim)
            fprintf(stderr,"Error: size of id_list(%d) < nrimages(%d)\n",id_list.size(),nrim);

        std::vector<ncCommand> cmd_list;


        for (int i=0;i<nrim-1;++i)
        {
            cmd_list.push_back(ncCommand(id_list[i].cid,id_list[i].vid,ld__,-1));
            ld__=(ld__+1)%nrim;
        }

        cmd_list.push_back(ncCommand(id_list[nrim-1].cid,id_list[nrim-1].vid,ld__,ref__));
        ld__=(ld__+1)%nrim;
        ref__=(ref__+1)%nrim;

        for (int i=0;i<flow.imrange;++i)
        {
            cmd_list.push_back(ncCommand(-1,-1,-1,ref__));
            ref__=(ref__+1)%nrim;
        }

        for (int i=nrim;i<id_list.size();++i)
        {
            cmd_list.push_back(ncCommand(id_list[i].cid,id_list[i].vid,ld__,ref__));
            ld__=(ld__+1)%nrim;
            ref__=(ref__+1)%nrim;
        }

        for (int i=0;i<flow.imrange;++i)
        {
            cmd_list.push_back(ncCommand(-1,-1,-1,ref__));
            ref__=(ref__+1)%nrim;
        }

#if 0
        fprintf(stderr,"cmd_list = \n");
        for (unsigned int i=0;i<cmd_list.size();++i)
            cmd_list[i].print();
#endif

        return cmd_list;
    }

    ncc::RTd getTransform() {
        return project.poses.poselist->getTransform();
    }

    ncc::RTd getInvTransform() {
        return project.poses.poselist->getTransform().inverse();
    }

    ncCalibration getCalibration(int cid , int vid) {
        ncCalibration caltex = (*(project.cameras.calset))[cid];
        caltex.setMV((*(project.poses.poselist))[vid]*caltex.MV());
        return caltex;
    }

    ncTexInfo getTexInfo(int cid , int vid , int lod) {
        ncCalibration caltex = (*(project.cameras.calset))[cid];

        if (rectify) {

        	ncc::RTd MV	= ncc::RTd::getIdentity();

			switch (cid) {

        		case 12:
        		case 14:
				{
					MV[0][0] = -1.0;
					MV[0][1] =  0.0;
					MV[0][2] =  0.0;
					MV[1][0] =  0.0;
					MV[1][1] =  0.0;
					MV[1][2] = -1.0;
					MV[2][0] =  0.0;
					MV[2][1] = -1.0;
					MV[2][2] =  0.0;
					MV[3] 	 =  caltex.MV()[3];
					break;
				}
        		case 13:
        		case 15:
				{
					MV[0][0] =  1.0;
					MV[0][1] =  0.0;
					MV[0][2] =  0.0;
					MV[1][0] =  0.0;
					MV[1][1] =  0.0;
					MV[1][2] = -1.0;
					MV[2][0] =  0.0;
					MV[2][1] =  1.0;
					MV[2][2] =  0.0;
					MV[3] 	 =  caltex.MV()[3];
					break;
				}
			}

			ncc::m4x4d tr = MV.inverse()*caltex.MV()*caltex.Ktex(0).inverse();

			double xmin = 0.0;
			double xmax = 0.0;
			double ymin = 0.0;
			double ymax = 0.0;
			double xval,yval;

			ncc::v4d tmp1;

			tmp1 = tr*ncc::v4d(0.0,0.0,0.0,1.0);
			xval = tmp1.x/tmp1.z;
			yval = tmp1.y/tmp1.z;
			xmin = std::min(xval,xmin);
			ymin = std::min(yval,ymin);
			xmax = std::max(xval,xmax);
			ymax = std::max(yval,ymax);

			tmp1 = tr*ncc::v4d(1.0,0.0,0.0,1.0);
			xval = tmp1.x/tmp1.z;
			yval = tmp1.y/tmp1.z;
			xmin = std::min(xval,xmin);
			ymin = std::min(yval,ymin);
			xmax = std::max(xval,xmax);
			ymax = std::max(yval,ymax);

			tmp1 = tr*ncc::v4d(0.0,1.0,0.0,1.0);
			xval = tmp1.x/tmp1.z;
			yval = tmp1.y/tmp1.z;
			xmin = std::min(xval,xmin);
			ymin = std::min(yval,ymin);
			xmax = std::max(xval,xmax);
			ymax = std::max(yval,ymax);

			tmp1 = tr*ncc::v4d(1.0,1.0,0.0,1.0);
			xval = tmp1.x/tmp1.z;
			yval = tmp1.y/tmp1.z;
			xmin = std::min(xval,xmin);
			ymin = std::min(yval,ymin);
			xmax = std::max(xval,xmax);
			ymax = std::max(yval,ymax);

			double A = -(xmax+xmin)/(xmax-xmin);
			double B = -(ymax+ymin)/(ymax-ymin);

			ncc::m4x4d KG;
			KG[0] = ncc::v4d(2.0/(xmax-xmin) 	, 0.0				, 0.0 ,0.0 );
			KG[1] = ncc::v4d(0.0				, 2.0/(ymax-ymin) 	, 0.0 ,0.0 );
			KG[2] = ncc::v4d(A 					, B				 	, 0.0 ,1.0 );
			KG[3] = ncc::v4d(0.0				, 0.0				, 1.0 ,0.0 );

			ncc::m4x4d KN = ncc::PGLtoP(KG,(int)caltex.width(0),(int)caltex.height(0));
			KN[0][2] = caltex.K()[0][2];
			KN[1][2] = caltex.K()[1][2];
			KN[2][2] = caltex.K()[2][2];
			KN[3][2] = caltex.K()[3][2];
			caltex.setMV(MV);
			caltex.setK(KN);

        }

        caltex.setMV((*(project.poses.poselist))[vid]*caltex.MV());

		ncTexInfo texinfo;
		texinfo.cid     = cid;
		texinfo.vid     = vid;
		texinfo.lod     = lod;
		texinfo.calib   = caltex;

        return texinfo;
    }

    bool getVanPosition(int vid , ncc::v3d & vp) {
        if (project.poses.poselist->isValid(vid))
        {
            vp = (*(project.poses.poselist))[vid][3].xyz();
            return true;
        }
        else
            return false;
    }

    std::vector<int> getActiveCids() {
		std::vector<int> acids;
		for (int i=0;i<MAX_CAM;++i)
			if (project.cameras.active[i])
				acids.push_back(i);
		return acids;
	}

    void getBoundingBox( ncc::v3d & pmin3 , ncc::v3d & pmax3) {

        if (vox.bb_mode == NC_BBOX_MANUAL)
        {
            //fprintf(stderr,"vox.has_bb\n");
            pmin3 = vox.bb_min;
            pmax3 = vox.bb_max;
        }
        else if (vox.bb_mode == NC_BBOX_AUTO)
        {
           if (getActiveCids().size()>0)
            {
                ncc::v3d    pmin,pmax,ptmp;

                int vid = project.poses.start_pose;
                while ((vid<=project.poses.end_pose) && !getVanPosition(vid,ptmp))
                    vid++;

                if (vid<=project.poses.end_pose)
                {
                    pmin = pmax = ptmp;

                    while (vid<=project.poses.end_pose)
                    {
                        if (getVanPosition(vid,ptmp))
                        {
                            pmin = (ncc::min)(pmin,ptmp);
                            pmax = (ncc::max)(pmax,ptmp);
                        }
                        vid++;
                    }
                    pmin3 = pmin - ncc::v3f(20.0f,20.0f,3.0f);
                    pmax3 = pmax + ncc::v3f(20.0f,20.0f,3.0f);
                }
                else
                    fprintf(stderr,"Error: no valid pose found for bounding box...\n");

                //fprintf(stderr,"pmin3 = %25.20f , %25.20f ,%25.20f\n" ,pmin3.x,pmin3.y,pmin3.z );
                //fprintf(stderr,"pmax3 = %25.20f , %25.20f ,%25.20f\n" ,pmax3.x,pmax3.y,pmax3.z );
            }
            else
            {
                fprintf(stderr,"Error: no Flow cameras defined...\n");
                pmin3 = pmax3 = ncc::v3d(0.0,0.0,0.0);
            }
        }
    }

    //! pgf image

    bool loadImage(ncPtr<ncTexImage2D>& tex, unsigned int cid, unsigned int pid, bool use_mask) {

        ncImageBuffer<ncc::v4ub> imbuf;
        if (!project.images.im_info.load(imbuf,cid,pid))
            return false;

        //ncCalibration calib_tex = getCalibration(cid, pid);
        //ncTexImage2D* tmptex;
        //pgftex = new ncTexImage2D(GL_RGBA8,imbuf.width,imbuf.height);
        //tmptex = new ncTexImage2D(GL_RGBA8,imbuf.width,imbuf.height);
        //compensateVignetting(imbuf);
        //tmptex->setData(imbuf.data);
        //ncgl::undistorter->undistort(tmptex,pgftex,calib_tex);
        //pgftex->generateMipmap();
        //delete tmptex;

        if (!tex)
            tex = new ncTexImage2D(GL_RGBA8,imbuf.width,imbuf.height);
        tex->resize(imbuf.width,imbuf.height);
        tex->setData(imbuf.data);

        return true;
    }

    //! image buffer

    template <class T>
    void loadImage(cp_enum cp , unsigned int cid, unsigned int vid , ncImageBuffer<T>& imdata , ncTexInfo & texinfo) {

        std::string fname = getFileName(cp,cid,vid);


        std::ifstream cfile(fname.c_str());
        if (cfile.fail())
        {
            fprintf(stderr,"loadImage error: could not find %s...\n",fname.c_str());
            return;
        }

        texinfo.read(cfile);
        imdata.resize(texinfo.width(),texinfo.height());
        cfile.read((char *)&imdata[0].x        , imdata.width*imdata.height*sizeof(T));
        cfile.close();
    }

    template <class T>
    void saveImage(cp_enum cp , ncImageBuffer<T>& imdata , ncTexInfo& texinfo) {

        std::string fname = getFileName(cp,texinfo.cid,texinfo.vid);
        fprintf(stderr,"saving to %s (%d,%d)\n",fname.c_str(),texinfo.cid,texinfo.vid);

        std::ofstream cfile(fname.c_str());
        texinfo.write(cfile);
        cfile.write((char *)&imdata[0].x        , imdata.width*imdata.height*sizeof(T));
        cfile.close();
    }

    //! texture 2D

    template <class T>
    void loadImage(cp_enum cp , ncPtr<ncTexImage2D>& tex, unsigned int cid, unsigned int vid , ncTexInfo & texinfo) {

        ncImageBuffer<T> imdata;
        loadImage<T>(cp , cid , vid , imdata , texinfo);

        if (!tex)
        {
            tex = new ncTexImage2D(my_gl_enum<T>().iFormat(),imdata.width,imdata.height);
        }
        else if (tex->internalformat != my_gl_enum<T>().iFormat())
        {
            fprintf(stderr,"loadImage error: texture internal format error...\n");
            return;
        }
        else
            tex->resize(texinfo.width(),texinfo.height());

        tex->setData(&imdata[0].x);
    }

    template <class T>
    void saveImage(cp_enum cp, ncPtr<ncTexImage2D>& tex, int texlod , int cid , int vid , int lod) {

        ncTexInfo texinfo = getTexInfo(cid,vid,lod);

        if (((tex->width>>texlod) != texinfo.width()) || ((tex->height>>texlod) != texinfo.height()))
        {
            fprintf(stderr,"saveImage: Dimension error!\n");
            //fprintf(stderr,"test = %dx%d\n",(tex->width)>>texlod,(tex->height)>>texlod);
            return;
        }

        tex->bind(0);

        ncImageBuffer<T> imdata(texinfo.width(),texinfo.height());
        glGetTexImage(tex->target,texlod,my_gl_enum<T>().format() , my_gl_enum<T>().type() , &imdata[0].x);

        saveImage<T>(cp , imdata , texinfo);

    }

    //! texture 2D array

    template <class T>
    void loadImage(cp_enum cp , ncPtr<ncTexImage2DArray>& tex, unsigned int index, unsigned int cid, unsigned int vid , ncTexInfo & texinfo) {

        ncImageBuffer<T> imdata;
        loadImage<T>(cp , cid , vid , imdata , texinfo);

        if (!tex)
        {
            fprintf(stderr,"loadImage error: texture not initialized...\n");
            return;
        }
        else if (tex->internalformat != my_gl_enum<T>().iFormat())
        {
            fprintf(stderr,"loadImage error: texture internal format error...\n");
            return;
        }
        else if ((texinfo.width()!=imdata.width) || (texinfo.height()!=imdata.height) || (index>=tex->depth))
        {
            fprintf(stderr,"loadImage error: dimension error...\n");
            return;
        }

        tex->setData(index,&imdata[0].x);
    }

    //! texture viewer
    void drawTexture(tv_enum my_enum, unsigned int cid, unsigned int vid) {

        ncTexInfo           texinfo;
        ncPtr<ncTexImage2D> tex;
        ncc::m4x4f          cm;
        ncc::v4f            co(0.0f,0.0f,0.0f,0.0f);

        glDisable(GL_DEPTH_TEST);

        switch (my_enum) {
            case TV_COLOR:
                loadImage<ncc::v4ub>(COLOR_CP , tex, cid, vid , texinfo);
                cm.setIdentity();
                break;
            case TV_DEPTH:
                loadImage<ncc::v2f>(DEPTH_CP , tex, cid, vid , texinfo);
                cm[0]=ncc::v4f(1.0f,1.0f,1.0f,1.0f);
                cm[1]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[2]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[3]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                break;
            case TV_MASK:
                loadImage<ncc::v2f>(DEPTH_CP , tex, cid, vid , texinfo);
                cm[0]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[1]=ncc::v4f(1.0f,1.0f,1.0f,1.0f);
                cm[2]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[3]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                break;
            case TV_DEPTH_CHECK:
                loadImage<ncc::v2f>(DEPTH_CHECK_CP , tex, cid, vid , texinfo);
                cm[0]=ncc::v4f(1.0f,1.0f,1.0f,1.0f);
                cm[1]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[2]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[3]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                break;
            case TV_MASK_CHECK:
                loadImage<ncc::v2f>(DEPTH_CHECK_CP , tex, cid, vid , texinfo);
                cm[0]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[1]=ncc::v4f(1.0f,1.0f,1.0f,1.0f);
                cm[2]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                cm[3]=ncc::v4f(0.0f,0.0f,0.0f,0.0f);
                break;
            default:
                fprintf(stderr,"Error: Unknown tv enumerator...\n");
                break;
        }

        if (tex)
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            tex->draw(cm,co,true);
        }
        else
        {
            glClearColor(1.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

    }

    void loadTracks(int min_pose, int max_pose) {

        if (project.tracks.tracklist)
            delete project.tracks.tracklist;
        project.tracks.tracklist = new ncTrackList();
        project.tracks.tracklist->readTracks(project.tracks.directory, project.tracks.regexp , min_pose, max_pose);
    }

	bool			valid;
    std::string     file_name;
    std::string     work_dir;
    tv_enum         my_tv_enum;
    bool			rectify;
    ncConfigProject project;
    ncConfigOrtho 	ortho;
    ncConfigFlow    flow;
    ncConfigVox     vox;

};

#endif //__NC_CONFIG_H__
