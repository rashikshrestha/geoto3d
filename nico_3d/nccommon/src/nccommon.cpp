#include "nccommon.h"

#include "nccalibrationset.h"
#include "ncposelist.h"

bool ncDebug::use_tabs  = true;
int  ncDebug::nr_tabs   = 0;

using namespace std;


ncDebug::ncDebug(std::string name) : name(name) {

    if (use_tabs)
        for (int i=0;i<nr_tabs;++i)
            fprintf(stderr,"\t");
    fprintf(stderr,"+%s\n",name.c_str());
    nr_tabs++;
}

ncDebug::~ncDebug() {
    nr_tabs--;
    if (use_tabs)
        for (int i=0;i<nr_tabs;++i)
            fprintf(stderr,"\t");
    fprintf(stderr,"-%s\n",name.c_str());
}


ImageInfo::ImageInfo() : directory("") , valid(false) {

}

ImageInfo::~ImageInfo() {

}

ImageInfo::ImageInfo(std::string im_dir , ncCalibrationSet& camset , ncPoseList& poses   ) {
    setup(im_dir , camset , poses);
}

ImageInfo::ImageInfo(std::string im_dir , std::string camsetfile   , std::string posefile) {
    setup(im_dir , camsetfile , posefile);
}

void ImageInfo::setup(std::string im_dir , ncCalibrationSet& camset , ncPoseList& poses   ) {

	bool found_at_least_one = false;

	char str[1024];

	for (unsigned int cid=0;cid<MAX_CAM;++cid)
	{
        sprintf(str,"%s/%02d",im_dir.c_str(),cid);
		if (!ncc::checkDir(str))
		{
			cam_info[cid].valid  = false;
            continue;
		}

		cam_info[cid].valid  = true;
		cam_info[cid].width  = camset[cid].width(0);
		cam_info[cid].height = camset[cid].height(0);

		std::string fname;
        if (ncc::getAnyImageFile(im_dir,cid,poses.getMinVid(),poses.getMaxVid(),fname)) {

            found_at_least_one = true;

            std::string::size_type idx1,idx2;
            idx1 = fname.rfind('.');
            if (idx1 == std::string::npos) break;
            std::string tmp = fname.substr(0,idx1);
            idx2 = tmp.rfind('.');
            if (idx2 == std::string::npos) break;
            unsigned int pose_digits = idx1-idx2-1;
            std::string file_ext = fname.substr(idx1+1);

            sprintf(str,"%s/%02d/image.%%0%dd.%s",im_dir.c_str(),cid,pose_digits,file_ext.c_str());
            cam_info[cid].regexp = std::string(str);
            if ( (strcmp(file_ext.c_str(),"jpg")==0))
            {
                cam_info[cid].jpg    = true;
                cam_info[cid].color  = true;
                cam_info[cid].hdr    = false;
            }
            else
            {
                cam_info[cid].jpg    = false;
                cam_info[cid].color  = !ncc::isGray(fname);
                cam_info[cid].hdr    = ncc::isHDR(fname);
            }
        }
	}

    valid       = found_at_least_one;
    directory   = valid ? im_dir : "";
}

void ImageInfo::setup(std::string im_dir , std::string camsetfile   , std::string posefile) {

    if (!(ncc::checkFile(camsetfile) && ncc::checkFile(posefile)))
        valid = false;
    else {
        ncCalibrationSet    cam_set(camsetfile);
        ncPoseList          pose_list(posefile);
        setup(im_dir,cam_set,pose_list);
    }
}

void ImageInfo::print() {
    fprintf(stderr,"imagefiledir : %s\n",directory.c_str());
    for (unsigned int i=0;i<MAX_CAM;++i)
        fprintf(stderr,"cid[%2d] hdr[%d] color[%d] jpg[%d] valid[%d] size=%dx%d\n",i,cam_info[i].hdr,cam_info[i].color,cam_info[i].jpg,cam_info[i].valid,cam_info[i].width,cam_info[i].height);
}

bool ImageInfo::load( ncImageBuffer<ncc::v4ub> & imbuf , unsigned int cid, unsigned int pid , unsigned int level /*=0*/, int border /*=1*/ ) {

    //fprintf(stderr,"loading level %d\n",level);

    std::string fname = getFileName(cid , pid);

    if (!ncc::checkFile(fname.c_str()))
        return false;

    if (cam_info[cid].jpg) {
        //! JPG
        ncc::readImage(fname, imbuf, level , false);
    }
    else
    {
        //! PGF
        if (cam_info[cid].hdr)
        {
            //! HDR
            if (cam_info[cid].color)
            {
                //! Color
                //fprintf(stderr,"Loading Color HDR image %s...\n",fname.c_str());
                ncImageBuffer<ncc::v3us> pgfbuf;
                ncc::readPGF(fname.c_str(), pgfbuf, level , false );
                imbuf.resize(pgfbuf.width,pgfbuf.height);

                for (unsigned int i=0;i<pgfbuf.width*pgfbuf.height;++i)
                {
                     unsigned char r = (std::min)(255,(std::max)(0,pgfbuf[i].x>>8));
                     unsigned char g = (std::min)(255,(std::max)(0,pgfbuf[i].y>>8));
                     unsigned char b = (std::min)(255,(std::max)(0,pgfbuf[i].z>>8));
                     imbuf[i] = ncc::v4ub(r,g,b,255);
                }
            }
            else
            {
                //! Grayscale
                //fprintf(stderr,"Loading Grayscale HDR image %s...\n",fname.c_str());
                ncImageBuffer<unsigned short> pgfbuf;
                ncc::readPGF(fname.c_str(), pgfbuf, level , false );
                imbuf.resize(pgfbuf.width,pgfbuf.height);

                for (unsigned int i=0;i<pgfbuf.width*pgfbuf.height;++i)
                {
                    unsigned char intensity = (std::min)(255,(std::max)(0,pgfbuf[i]>>8));
                    imbuf[i] = ncc::v4ub(intensity,intensity,intensity,255);
                }
            }
        }
        else
        {
            //! LDR
            if (cam_info[cid].color)
            {
                //! Color
                //fprintf(stderr,"Loading Color LDR image %s...\n",fname.c_str());
                ncImageBuffer<ncc::v3ub> pgfbuf;
                ncc::readPGF(fname.c_str(), pgfbuf, level , false );
                imbuf.resize(pgfbuf.width,pgfbuf.height);

                for (unsigned int i=0;i<pgfbuf.width*pgfbuf.height;++i)
                    imbuf[i] = ncc::v4ub(pgfbuf[i].x,pgfbuf[i].y,pgfbuf[i].z,255);
            }
            else
            {
                //! Grayscale
                //fprintf(stderr,"Loading Grayscale LDR image %s...\n",fname.c_str());
                ncImageBuffer<unsigned char> pgfbuf;
                ncc::readPGF(fname.c_str(), pgfbuf, level , false );
                imbuf.resize(pgfbuf.width,pgfbuf.height);

                for (unsigned int i=0;i<pgfbuf.width*pgfbuf.height;++i)
                    imbuf[i] = ncc::v4ub(pgfbuf[i],pgfbuf[i],pgfbuf[i],255);
            }
        }
    }

/*
    char maskfile[1024];
    if (use_mask && project.cameras.getMaskName(maskfile,cid))
    {
        ncImageBuffer<unsigned char> maskbuf;
        ncc::readImage(maskfile,maskbuf);
        if ((maskbuf.width == imbuf.width) && (maskbuf.height == imbuf.height))
        {
            for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
                imbuf[i].w = maskbuf[i];
        }
        else
        {
            fprintf(stderr,"ERROR: camera mask dimension does not match image size!\n");
            for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
                imbuf[i].w = 255;
        }
    }
    else
    {
        for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
            imbuf[i].w = 255;
    }
*/

    for (int a=0;a<border;++a)
    {
        for (unsigned int i=0;i<imbuf.width;++i)
        {
            imbuf[a*imbuf.width+i].w = 0;
            imbuf[(imbuf.height-1-a)*imbuf.width+i].w = 0;
        }
        for (unsigned int i=0;i<imbuf.height;++i)
        {
            imbuf[i*imbuf.width+a].w = 0;
            imbuf[i*imbuf.width+imbuf.width-1-a].w = 0;
        }
    }

    return true;
}

bool ImageInfo::loadThumbnail( ncImageBuffer<ncc::v4ub>& imbuf , unsigned int cid, unsigned int pid , unsigned int minimum_nr_of_pixels , int border /*= 1*/ ) {

    int w = cam_info[cid].width;
    int h = cam_info[cid].height;

    int level       = 0;
    while (w*h>=minimum_nr_of_pixels) {
        w = (w+1)>>1;
        h = (h+1)>>1;
        level++;
    }
    level--;

    return load( imbuf , cid, pid , level , border );
}

std::string ImageInfo::getFileName(unsigned int cid,unsigned int pid) {
    char fname[1024];
    sprintf(fname,cam_info[cid].regexp.c_str(), pid);
    return std::string(fname);
}

std::string
ncc::getDirName(std::string fname) {
#ifdef _WIN32
    char drive[1024];
	char dir[1024];
	char fname_out[1024];
	char ext[1024];

	_splitpath(fname.c_str(),drive,dir,fname_out,ext);

	char path[1024];
	sprintf(path,"%s/%s",drive,dir);
	return path;
#else
    std::string tmp = fname;
    tmp = dirname(&tmp[0]);
    return tmp;
#endif
}

std::string
ncc::getFileName(std::string fname) {
#ifdef _WIN32
    char drive[1024];
	char dir[1024];
	char fname_out[1024];
	char ext[1024];

	_splitpath(fname.c_str(),drive,dir,fname_out,ext);
	//fprintf(stderr,"getFileName %s -> %s\n", fname.c_str() , fname_out);
	return std::string(fname_out) + std::string(ext);
#else
    std::string tmp = fname;
    tmp = basename(&tmp[0]);
    return tmp;
#endif
}

std::vector<std::string> &
ncc::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string>
ncc::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<std::string>
ncc::getFiles(std::string path, std::string pattern) {

	std::vector<std::string> filenames;
	filenames.clear();

	std::string regexp = path + "/" + pattern;

#ifdef _WIN32

   WIN32_FIND_DATA FileInformation;
   HANDLE hFile = FindFirstFileEx(regexp.c_str(), (_FINDEX_INFO_LEVELS)1, &FileInformation, FindExSearchNameMatch, NULL, 0x1);
   if(hFile != INVALID_HANDLE_VALUE)
   {
      do
      {
         if(FileInformation.cFileName[0] != '.')
         {
			 filenames.push_back((path + "/" + FileInformation.cFileName).c_str());
         }
      } while(::FindNextFile(hFile, &FileInformation) == TRUE);

      ::FindClose(hFile);
   }
#else

	glob_t globbuf;
	int r = glob(regexp.c_str(), GLOB_ERR | GLOB_MARK, NULL, &globbuf);

	for (unsigned int i = 0; i < globbuf.gl_pathc; ++i)
		filenames.push_back(globbuf.gl_pathv[i]);

	globfree(&globbuf);
#endif

    return filenames;

}

bool ncc::checkFile(std::string fname, std::string* msg) {

#ifdef _WIN32

	DWORD dwAttr = GetFileAttributes(fname.c_str());
	if(dwAttr == 0xffffffff)
	{

	  DWORD dwError = GetLastError();
	  if(dwError == ERROR_FILE_NOT_FOUND)
	  {
		if (msg)
		{
			std::stringstream strstr;
			strstr << "File " << fname.c_str() << " not found\n";
			*msg = strstr.str();
		}
	  }
	  else if(dwError == ERROR_PATH_NOT_FOUND)
	  {
		if (msg)
		{
			std::stringstream strstr;
			strstr << "Path for file " << fname.c_str() << " not found\n";
			*msg = strstr.str();
		}
	  }
	  else if(dwError == ERROR_ACCESS_DENIED)
	  {
		if (msg)
		{
			std::stringstream strstr;
			strstr << "Access to file " << fname.c_str() << " denied\n";
			*msg = strstr.str();
		}
	  }
	  else
	  {
		if (msg)
		{
			std::stringstream strstr;
			strstr << "Unknown error when accessing file " << fname.c_str() << "\n";
			*msg = strstr.str();
		}
	  }
	  return false;
	}
	else
	{
	  if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	  {
		return false;
	  }
	  else
	  {
		return true;
	  }
	}


#else


	struct stat stFileInfo;
	if (stat(fname.c_str(),&stFileInfo))
	{
		if (msg)
		{
			std::stringstream strstr;
			strstr << "File " << fname.c_str() << " does not exist\n";
			*msg = strstr.str();
		}
		return false;
	}
	else
		if (S_ISREG(stFileInfo.st_mode))
			return true;
		else
		{
			if (msg)
			{
				std::stringstream strstr;
				strstr << fname.c_str() << " is not a regular file\n";
				*msg = strstr.str();
			}
			return false;
		}

#endif

}

std::string ncc::getFileExt(std::string fname) {
    std::string::size_type idx = fname.rfind('.');
    if (idx == std::string::npos)
        return "";
    else
        return fname.substr(idx+1);
}

std::string ncc::changeFileExt(std::string fname , std::string new_ext) {
    std::string::size_type idx = fname.rfind('.');
    if (idx == std::string::npos)
        return fname + "." + new_ext;
    else
        return fname.substr(0,idx+1)+new_ext;
}

std::string ncc::getFileBaseName(std::string fname) {
    std::string::size_type idx1	= fname.rfind('/');
	if (idx1 == std::string::npos)
		idx1 = 0;
    std::string::size_type idx2	= fname.rfind('.');

    return fname.substr(idx1+1,idx2-idx1-1);
}

bool ncc::checkFileExt(std::string fname,std::string ext) {

    std::string fext = getFileExt(fname);
    bool icomp = !strcasecmp(fext.c_str(),ext.c_str());
    return icomp;
}

bool ncc::checkDir(std::string dname, std::string* msg) {

#ifdef _WIN32

	DWORD dwAttr = GetFileAttributes(dname.c_str());
	if(dwAttr == 0xffffffff)
	{
		return false;
	}
	else
	{
	  if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	  {
		return true;
	  }
	  else
	  {
		return false;
	  }
	}


#else


	struct stat stDirInfo;
	if (stat(dname.c_str(),&stDirInfo))
	{
		if (msg)
		{
			std::stringstream strstr;
			strstr << "Directory " << dname.c_str() << " does not exist\n";
			*msg = strstr.str();
		}
		return false;
	}
	else
		if (S_ISDIR(stDirInfo.st_mode))
			return true;
		else
		{
			if (msg)
			{
				std::stringstream strstr;
				strstr << dname.c_str() << " is not a directory\n";
				*msg = strstr.str();
			}
			return false;
		}


#endif

}

bool ncc::createDir(std::string dname, std::string* msg) {

#ifdef _WIN32
	CreateDirectory (dname.c_str(), NULL);
#else
	char syscall[1024];
	sprintf(syscall,"mkdir -p \"%s\"",dname.c_str());
	system(syscall);
#endif
	return ncc::checkDir(dname);
}

bool ncc::getAnyImageFile(std::string image_dir , unsigned int cid , unsigned int pid_min , unsigned int pid_max , std::string & fname) {

    //fprintf(stderr,"checking %s\n",image_dir.c_str());

    char str[1024];

    unsigned int nrposes_minus_one = pid_max-pid_min;

    unsigned int step    = 1<<28; //! initial large step
    unsigned int index;
    unsigned int start_index = 0;

    for (index=start_index;index<=nrposes_minus_one;)
    {
        unsigned int real_pid = pid_min+index;

        sprintf(str,"%s/%02u/image.%06u.pgf",image_dir.c_str(),cid,real_pid);
        if (ncc::checkFile(str))
        {
            fname = str;
            return true;
        }
        sprintf(str,"%s/%02u/image.%06u.jpg",image_dir.c_str(),cid,real_pid);
        if (ncc::checkFile(str))
        {
            fname = str;
            return true;
        }
        sprintf(str,"%s/%02u/image.%09u.pgf",image_dir.c_str(),cid,real_pid);
        if (ncc::checkFile(str))
        {
            fname = str;
            return true;
        }
        sprintf(str,"%s/%02u/image.%09u.jpg",image_dir.c_str(),cid,real_pid);
        if (ncc::checkFile(str))
        {
            fname = str;
            return true;
        }

        if (index>0xFFFFFFFF-step)
            break;
        else
            index+=step;
    }
    start_index += step>>1;

    while (step>1)
    {
        for (index=start_index;index<=nrposes_minus_one;)
        {
            unsigned int real_pid = pid_min+index;

            sprintf(str,"%s/%02u/image.%06u.pgf",image_dir.c_str(),cid,real_pid);
            if (ncc::checkFile(str))
            {
                fname = str;
                return true;
            }
            sprintf(str,"%s/%02u/image.%06u.jpg",image_dir.c_str(),cid,real_pid);
            if (ncc::checkFile(str))
            {
                fname = str;
                return true;
            }
            sprintf(str,"%s/%02u/image.%09u.pgf",image_dir.c_str(),cid,real_pid);
            if (ncc::checkFile(str))
            {
                fname = str;
                return true;
            }
            sprintf(str,"%s/%02u/image.%09u.jpg",image_dir.c_str(),cid,real_pid);
            if (ncc::checkFile(str))
            {
                fname = str;
                return true;
            }

            if (index>0xFFFFFFFF-step)
                break;
            else
                index+=step;

        }

        start_index >>= 1;
        step        >>= 1;
    }

    return false;
}

bool ncc::getFirstFile(std::string dname , std::string pattern , std::string & fname) {

	std::string regexp = dname + "/" + pattern;

#ifdef _WIN32
	WIN32_FIND_DATA FileInformation;
	HANDLE hFile = FindFirstFileEx(regexp.c_str(), (_FINDEX_INFO_LEVELS)1, &FileInformation, FindExSearchNameMatch, NULL, 0x1);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.') {
				fname = (dname + "/" + FileInformation.cFileName).c_str();
				break;
			}
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);

      ::FindClose(hFile);
	}
#else

	glob_t globbuf;
	int r = glob(regexp.c_str(), GLOB_ERR | GLOB_MARK, NULL, &globbuf);
	fname = globbuf.gl_pathv[0];
	globfree(&globbuf);
#endif

    return (!fname.empty());
}

bool ncc::getFirstFile(std::string dname , std::string & fname , bool full_path) {

#ifdef _WIN32

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((dname+"/*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr,"FindFirstFile failed (%d)\n", GetLastError());
		return false;
	}
	else
	{
		fname = FindFileData.cFileName;
		FindClose(hFind);
		return true;
	}

#else

    DIR* pDirectory = opendir(dname.c_str());
    if (pDirectory)
    {
        bool found = false;

        struct stat     stFileInfo;
        struct dirent   *pFile = NULL;

        while (!found && (pFile = readdir(pDirectory)))
        {
            const std::string fp = dname+"/"+pFile->d_name;
            stat(fp.c_str(), &stFileInfo);
            if (S_ISREG(stFileInfo.st_mode))
            {
                found = true;
                fname = full_path ? fp : pFile->d_name;
            }
        }
        closedir(pDirectory);
        return found;
    }
    else
        return false;
#endif
}

bool ncc::readPGF( std::string fname, ncImageBuffer<unsigned char> & imbuf, int level, bool presized ) {

#ifdef _WIN32

    HANDLE hFile;
    hFile = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

#else

    int hFile = open(fname.c_str(), O_RDONLY);
    if (hFile == -1) {
        std::cerr << "LoadPGF: Can not open " << fname.c_str() << std::endl;
        return false;
    }
#endif

    CPGFImage* pgf = new CPGFImage();

    CPGFFileStream stream(hFile);

    try {
        // open pgf image
        pgf->Open(&stream);
        // read pgf image
        pgf->Read(level);
    } catch(IOException& ) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    } catch (...) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    }

    bool        correct_channel_map = false;
	int         channel_map[4]		= {2,1,0,3};

	unsigned int udata_len;
    const char* udata = (char*)pgf->GetUserData(udata_len);
	if (udata_len>0)
	{
		//fprintf(stderr,"udata(%d) = \"%s\"\n",udata_len,udata);
		correct_channel_map = ( std::string(udata).find("PGF_CORRECT_CHANNEL_MAP") != std::string::npos );
	}

    if (presized) {
        ncImageBuffer<unsigned char> src( pgf->Width(level) , pgf->Height(level));
        pgf->GetBitmap(src.width*sizeof(unsigned char), (UINT8 *)src.data, 8*sizeof(unsigned char),correct_channel_map ? channel_map : NULL);

        int minw = (std::min)(src.width ,imbuf.width );
        int minh = (std::min)(src.height,imbuf.height);

        unsigned char clearval = 0x00;

        for (int y=0;y<minh;++y)
        {
            for (int x=0;x<minw;++x)
                imbuf(x,y) = src(x,y);
            for (int x=minw;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
        }
        for (int y=minh;y<imbuf.height;++y)
            for (int x=0;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
    }
    else
    {
        int w = pgf->Width(level);
        int h = pgf->Height(level);
        imbuf.resize(w,h);
        pgf->GetBitmap(w*sizeof(unsigned char), (UINT8 *)imbuf.data, 8*sizeof(unsigned char),correct_channel_map ? channel_map : NULL);
    }

    delete pgf;

#ifdef _WIN32
    CloseHandle(hFile);
#else
    close(hFile);
#endif

    return true;
}

bool ncc::readPGF( std::string fname, ncImageBuffer<unsigned short> & imbuf, int level, bool presized ) {

#ifdef _WIN32

    HANDLE hFile;
    hFile = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

#else

    int hFile = open(fname.c_str(), O_RDONLY);
    if (hFile == -1) {
        std::cerr << "LoadPGF: Can not open " << fname.c_str() << std::endl;
        return false;
    }
#endif

    CPGFImage* pgf = new CPGFImage();

    CPGFFileStream stream(hFile);

    try {
        // open pgf image
        pgf->Open(&stream);
        // read pgf image
        pgf->Read(level);
    } catch(IOException& ) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    } catch (...) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    }

    if (presized) {
        ncImageBuffer<unsigned short> src( pgf->Width(level) , pgf->Height(level));
        pgf->GetBitmap(src.width*sizeof(unsigned short), (UINT8 *)src.data, 8*sizeof(unsigned short));

        int minw = (std::min)(src.width ,imbuf.width );
        int minh = (std::min)(src.height,imbuf.height);

        unsigned short clearval = 0x0000;

        for (int y=0;y<minh;++y)
        {
            for (int x=0;x<minw;++x)
                imbuf(x,y) = src(x,y);
            for (int x=minw;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
        }
        for (int y=minh;y<imbuf.height;++y)
            for (int x=0;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
    }
    else
    {
        int w = pgf->Width(level);
        int h = pgf->Height(level);
        imbuf.resize(w,h);
        pgf->GetBitmap(w*sizeof(unsigned short), (UINT8 *)imbuf.data, 8*sizeof(unsigned short));
    }

    delete pgf;

#ifdef _WIN32
    CloseHandle(hFile);
#else
    close(hFile);
#endif

    return true;
}

bool ncc::readPGF( std::string fname, ncImageBuffer<ncc::v3ub> & imbuf, int level, bool presized ) {

#ifdef _WIN32

    HANDLE hFile;
    hFile = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

#else
    int hFile = open(fname.c_str(), O_RDONLY);
    if (hFile == -1) {
        std::cerr << "LoadPGF: Can not open " << fname << std::endl;
        return false;
    }
#endif

    CPGFImage* pgf = new CPGFImage();

    CPGFFileStream stream(hFile);

    try {
        // open pgf image
        pgf->Open(&stream);
        // read pgf image
        pgf->Read(level);
    } catch(IOException& ) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        //delete pgf;
        return false;
        } catch (...) {
            std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    }

    bool        correct_channel_map = false;
	int         channel_map[4]		= {2,1,0,3};

	unsigned int udata_len;
    const char* udata = (char*)pgf->GetUserData(udata_len);
	if (udata_len>0)
	{
		//fprintf(stderr,"udata(%d) = \"%s\"\n",udata_len,udata);
		correct_channel_map = ( std::string(udata).find("PGF_CORRECT_CHANNEL_MAP") != std::string::npos );
	}

    if (presized) {
        ncImageBuffer<ncc::v3ub> src( pgf->Width(level) , pgf->Height(level));
        pgf->GetBitmap(src.width*sizeof(ncc::v3ub), (UINT8 *)src.data, 8*sizeof(ncc::v3ub) , correct_channel_map ? channel_map : NULL);

        int minw = (std::min)(src.width ,imbuf.width );
        int minh = (std::min)(src.height,imbuf.height);

        ncc::v3ub clearval;

        for (int y=0;y<minh;++y)
        {
            for (int x=0;x<minw;++x)
                imbuf(x,y) = src(x,y);
                for (int x=minw;x<imbuf.width;++x)
                    imbuf(x,y) = clearval;
        }
        for (int y=minh;y<imbuf.height;++y)
            for (int x=0;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
    }
    else
    {
        int w = pgf->Width(level);
        int h = pgf->Height(level);
        imbuf.resize(w,h);
        pgf->GetBitmap(w*sizeof(ncc::v3ub), (UINT8 *)imbuf.data, 8*sizeof(ncc::v3ub) , correct_channel_map ? channel_map : NULL);
    }

    delete pgf;

#ifdef _WIN32
    CloseHandle(hFile);
#else
    close(hFile);
#endif

    return true;
}

bool ncc::readPGF( std::string fname, ncImageBuffer<ncc::v4ub> & imbuf, int level, bool presized ) {

#ifdef _WIN32

    HANDLE hFile;
    hFile = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

#else
    int hFile = open(fname.c_str(), O_RDONLY);
    if (hFile == -1) {
        std::cerr << "LoadPGF: Can not open " << fname << std::endl;
        return false;
    }
#endif

    CPGFImage* pgf = new CPGFImage();

    CPGFFileStream stream(hFile);

    try {
        // open pgf image
        pgf->Open(&stream);
        // read pgf image
        pgf->Read(level);
    } catch(IOException& ) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    } catch (...) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    }

    bool        correct_channel_map = false;
	int         channel_map[4]		= {2,1,0,3};

	unsigned int udata_len;
    const char* udata = (char*)pgf->GetUserData(udata_len);
	if (udata_len>0)
	{
		//fprintf(stderr,"udata(%d) = \"%s\"\n",udata_len,udata);
		correct_channel_map = ( std::string(udata).find("PGF_CORRECT_CHANNEL_MAP") != std::string::npos );
	}

    if (presized) {
        ncImageBuffer<ncc::v4ub> src( pgf->Width(level) , pgf->Height(level));
        pgf->GetBitmap(src.width*sizeof(ncc::v4ub), (UINT8 *)src.data, 8*sizeof(ncc::v4ub),correct_channel_map ? channel_map : NULL);

        int minw = (std::min)(src.width ,imbuf.width );
        int minh = (std::min)(src.height,imbuf.height);

        ncc::v4ub clearval;

        for (int y=0;y<minh;++y)
        {
            for (int x=0;x<minw;++x)
                imbuf(x,y) = src(x,y);
            for (int x=minw;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
        }
        for (int y=minh;y<imbuf.height;++y)
            for (int x=0;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
    }
    else
    {
        int w = pgf->Width(level);
        int h = pgf->Height(level);
        imbuf.resize(w,h);
        pgf->GetBitmap(w*sizeof(ncc::v4ub), (UINT8 *)imbuf.data, 8*sizeof(ncc::v4ub),correct_channel_map ? channel_map : NULL);
    }

    delete pgf;

#ifdef _WIN32
    CloseHandle(hFile);
#else
    close(hFile);
#endif

    return true;
}

bool ncc::readPGF(std::string fname, ncImageBuffer<ncc::v3us> & imbuf, int level, bool presized) {

#ifdef _WIN32

    HANDLE hFile;
    hFile = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

#else
    int hFile = open(fname.c_str(), O_RDONLY);
    if (hFile == -1) {
        std::cerr << "LoadPGF: Can not open " << fname << std::endl;
        return false;
    }
#endif

    CPGFImage* pgf = new CPGFImage();

    CPGFFileStream stream(hFile);

    try {
        // open pgf image
        pgf->Open(&stream);
        // read pgf image
        pgf->Read(level);
    } catch(IOException& ) {
        std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        //delete pgf;
        return false;
        } catch (...) {
            std::cerr << "Error in file " << fname.c_str() << std::endl;
#ifdef _WIN32
        CloseHandle(hFile);
#else
        close(hFile);
#endif
        delete pgf;
        return false;
    }

    bool        correct_channel_map = false;
	int         channel_map[4]		= {2,1,0,3};

	unsigned int udata_len;
    const char* udata = (char*)pgf->GetUserData(udata_len);
	if (udata_len>0)
	{
		//fprintf(stderr,"udata(%d) = \"%s\"\n",udata_len,udata);
		correct_channel_map = ( std::string(udata).find("PGF_CORRECT_CHANNEL_MAP") != std::string::npos );
	}

    if (presized) {
        ncImageBuffer<ncc::v3us> src( pgf->Width(level) , pgf->Height(level));
        pgf->GetBitmap(src.width*sizeof(ncc::v3us), (UINT8 *)src.data, 8*sizeof(ncc::v3us) , correct_channel_map ? channel_map : NULL);

        int minw = (std::min)(src.width ,imbuf.width );
        int minh = (std::min)(src.height,imbuf.height);

        ncc::v3us clearval;

        for (int y=0;y<minh;++y)
        {
            for (int x=0;x<minw;++x)
                imbuf(x,y) = src(x,y);
                for (int x=minw;x<imbuf.width;++x)
                    imbuf(x,y) = clearval;
        }
        for (int y=minh;y<imbuf.height;++y)
            for (int x=0;x<imbuf.width;++x)
                imbuf(x,y) = clearval;
    }
    else
    {
        int w = pgf->Width(level);
        int h = pgf->Height(level);
        imbuf.resize(w,h);
        pgf->GetBitmap(w*sizeof(ncc::v3us), (UINT8 *)imbuf.data, 8*sizeof(ncc::v3us) , correct_channel_map ? channel_map : NULL);
    }

    delete pgf;

#ifdef _WIN32
    CloseHandle(hFile);
#else
    close(hFile);
#endif

    /*
    for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
    {
        imbuf[i][0]=std::min(0xFFFF,imbuf[i][0]<<4);
        imbuf[i][1]=std::min(0xFFFF,imbuf[i][1]<<4);
        imbuf[i][2]=std::min(0xFFFF,imbuf[i][2]<<4);
    }
    */

    return true;
}

bool ncc::writePGF(std::string fname, ncImageBuffer<ncc::v3ub> & imbuf, int levels, int quality) {
    if (0 > quality || quality > MaxQuality)
        return false;

    CPGFImage pgf;

#ifdef _WIN32
	HANDLE hFile;
	hFile = CreateFile(fname.c_str(), GENERIC_WRITE, NULL, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return false;
#else

	int hFile = open(fname.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (hFile == -1)
        return false;
#endif


// create stream object
    CPGFFileStream stream(hFile);
// create new image object
    PGFHeader header;
    header.width = imbuf.width;
    header.height = imbuf.height;
    header.bpp = 8*sizeof(ncc::v3ub);
    header.channels = 3;
    header.quality = quality;
    header.mode = ImageModeRGBColor;
    header.nLevels = levels;

    try {
        pgf.SetHeader(header);
        pgf.ImportBitmap(imbuf.width*3, (UINT8 *)imbuf.data, header.bpp);
    } catch(IOException& ) {
        return false;
    }

    try {
        // write image to pgf-file
        pgf.Write(&stream/*,header.nLevels*/);
    } catch(IOException& ) {
#ifdef _WIN32
	CloseHandle(hFile);
#else
	close(hFile);
#endif
		return false;
    }
#ifdef _WIN32
	CloseHandle(hFile);
#else
	close(hFile);
#endif

    return true;
}

void ncc::delaunay2D(const std::vector<ncc::v2d> & vin, std::vector<ncc::v3i> & vout) {

    vout.clear();

    if (vin.size()>2) {

        struct triangulateio pointsIn, allTrianglesOut;

        pointsIn.numberofpointattributes	= 0;
        pointsIn.numberofsegments			= 0;
        pointsIn.numberofholes				= 0;
        pointsIn.numberofregions			= 0;
        pointsIn.pointmarkerlist			= (int *) NULL;
        pointsIn.numberofpoints				= vin.size();
        pointsIn.pointlist					= (REAL *) &vin[0][0];

        allTrianglesOut.pointlist			= (REAL *) NULL;
        allTrianglesOut.trianglelist		= (int  *) NULL;

        // Perform the triangulation: z for first index [0], p for constraints,
        // Q for quiet, B for no boundary markers in output, P for no segments in output
        triangulate("zQBP", &pointsIn, &allTrianglesOut, NULL);

        vout.resize(allTrianglesOut.numberoftriangles);
        memcpy(&vout[0][0],allTrianglesOut.trianglelist,allTrianglesOut.numberoftriangles*sizeof(ncc::v3i));

        free(allTrianglesOut.pointlist);
        free(allTrianglesOut.trianglelist);
    }
}

void ncc::delaunay3D(const std::vector<ncc::v3d> & vin, std::vector<ncc::v4i> & vout) {

	vout.clear();

	tetgenio in, out;
	in.firstnumber = 0;

	in.numberofpoints = vin.size();
	in.pointlist = new REAL[in.numberofpoints * 3];


	for (unsigned int i=0;i<vin.size();++i)
	{
		in.pointlist[3*i+0] = vin[i][0];
		in.pointlist[3*i+1] = vin[i][1];
		in.pointlist[3*i+2] = vin[i][2];
	};

	tetrahedralize("Q", &in, &out);

	vout.resize(out.numberoftetrahedra);

	for (unsigned int i=0;i<vout.size();++i)
		vout[i] = ncc::v4i(out.tetrahedronlist[4*i+0],out.tetrahedronlist[4*i+1],out.tetrahedronlist[4*i+2],out.tetrahedronlist[4*i+3]);

	for (unsigned int i=0;i<vout.size();++i)
	{
		ncc::v3d n  = ncc::cross(vin[vout[i][2]]-vin[vout[i][0]],vin[vout[i][1]]-vin[vout[i][0]]);

		if (ncc::dot(vin[vout[i][3]]-vin[vout[i][0]],n) > 0)
		{
			int tmp_i	= vout[i][1];
			vout[i][1]  = vout[i][2];
			vout[i][2]  = tmp_i;
		}
	}

}

void ncc::rectify(ncCalibration cal1_in , ncCalibration cal2_in, ncCalibration & cal1_out , ncCalibration & cal2_out, int imw, int imh, double near, double far) {

	ncc::v3d baseline;
	ncc::v3d ortho;
	ncc::v3d newdir;

	ncc::v3d camcenter1 = (cal1_in.MV()*ncc::v4d(0.0,0.0,0.0,1.0)).xyz();
	ncc::v3d camcenter2 = (cal2_in.MV()*ncc::v4d(0.0,0.0,0.0,1.0)).xyz();
	ncc::v3d camdir1 	= (cal1_in.MV()*ncc::v4d(0.0,0.0,1.0,0.0)).xyz().normalize();
	ncc::v3d camdir2 	= (cal2_in.MV()*ncc::v4d(0.0,0.0,1.0,0.0)).xyz().normalize();

	newdir 		= (camdir1+camdir2).normalize();
	baseline 	= (camcenter2-camcenter1).normalize();
	ortho 		= ncc::cross(newdir,baseline).normalize();
	newdir 		= ncc::cross(baseline,ortho).normalize();

	ncc::RTd MV;
	MV.setIdentity();
	for (int i=0;i<3;++i)
	{
		MV[0][i] = baseline[i];
		MV[1][i] = ortho[i];
		MV[2][i] = newdir[i];
	}

	for (int i=0;i<3;++i)
		MV[3][i] = camcenter1[i];
	ncc::RTd MV1 = MV;

	for (int i=0;i<3;++i)
		MV[3][i] = camcenter2[i];
	ncc::RTd MV2 = MV;

	double minx,maxx,miny,maxy;

	ncc::v3d tl ,tr ,bl ,br ;
	ncc::v4d tlh,trh,blh,brh;

	//!first camera

	tlh =  MV1.inverse() * cal1_in.MV() * cal1_in.Ktex(0).inverse() * ncc::v4d(0.0,0.0,1.0,1.0);
	trh =  MV1.inverse() * cal1_in.MV() * cal1_in.Ktex(0).inverse() * ncc::v4d(1.0,0.0,1.0,1.0);
	blh =  MV1.inverse() * cal1_in.MV() * cal1_in.Ktex(0).inverse() * ncc::v4d(0.0,1.0,1.0,1.0);
	brh =  MV1.inverse() * cal1_in.MV() * cal1_in.Ktex(0).inverse() * ncc::v4d(1.0,1.0,1.0,1.0);

	for (int p=0;p<3;++p) {
		tl[p] = tlh[p];
		tr[p] = trh[p];
		bl[p] = blh[p];
		br[p] = brh[p];
	}

	tl/=tl.z;
	tr/=tr.z;
	bl/=bl.z;
	br/=br.z;

	minx = std::min(tl.x,std::min(tr.x,std::min(bl.x,br.x)));
	maxx = std::max(tl.x,std::max(tr.x,std::max(bl.x,br.x)));
	miny = std::min(tl.y,std::min(tr.y,std::min(bl.y,br.y)));
	maxy = std::max(tl.y,std::max(tr.y,std::max(bl.y,br.y)));

	//!second camera

	tlh =  MV2.inverse() * cal2_in.MV() * cal2_in.Ktex(0).inverse() * ncc::v4d(0.0,0.0,1.0,1.0);
	trh =  MV2.inverse() * cal2_in.MV() * cal2_in.Ktex(0).inverse() * ncc::v4d(1.0,0.0,1.0,1.0);
	blh =  MV2.inverse() * cal2_in.MV() * cal2_in.Ktex(0).inverse() * ncc::v4d(0.0,1.0,1.0,1.0);
	brh =  MV2.inverse() * cal2_in.MV() * cal2_in.Ktex(0).inverse() * ncc::v4d(1.0,1.0,1.0,1.0);

	for (int p=0;p<3;++p) {
		tl[p] = tlh[p];
		tr[p] = trh[p];
		bl[p] = blh[p];
		br[p] = brh[p];
	}

	tl/=tl.z;
	tr/=tr.z;
	bl/=bl.z;
	br/=br.z;

	minx = std::min(minx,std::min(tl.x,std::min(tr.x,std::min(bl.x,br.x))));
	maxx = std::max(maxx,std::max(tl.x,std::max(tr.x,std::max(bl.x,br.x))));
	miny = std::min(miny,std::min(tl.y,std::min(tr.y,std::min(bl.y,br.y))));
	maxy = std::max(maxy,std::max(tl.y,std::max(tr.y,std::max(bl.y,br.y))));

	ncc::m4x4d KK;
	KK[0][0] = 1.0/(maxx-minx);
	KK[1][0] = 0.0;
	KK[2][0] = -minx/(maxx-minx);
	KK[3][0] = 0.0;

	KK[0][1] = 0.0;
	KK[1][1] = 1.0/(maxy-miny);
	KK[2][1] = -miny/(maxy-miny);
	KK[3][1] = 0.0;

	KK[0][2] 	= 0.0;
	KK[1][2] 	= 0.0;
	KK[2][2] 	= 0.0;
	KK[3][2] 	= 0.0;

	KK[0][3] 	= 0.0;
	KK[1][3] 	= 0.0;
	KK[2][3] 	= 1.0;
	KK[3][3] 	= 0.0;

	KK = ncc::RTd::scale(imw,imh)*KK;

	cal1_out.setDepthRange(near,far);
	cal1_out.setImageSize(imw, imh);
	cal1_out.setK(KK);
	cal1_out.setMV(MV1);
	cal1_out.setKappa(cal1_in.kappa());

	cal2_out.setDepthRange(near,far);
	cal2_out.setImageSize(imw, imh);
	cal2_out.setK(KK);
	cal2_out.setMV(MV2);
	cal2_out.setKappa(cal2_in.kappa());
}

ncc::v4d ncc::fitToCanvas(double w_in, double h_in, double w_out, double h_out) {

		double dispw,disph,lb,bb;

		dispw = disph = lb = bb = 0.0;

		if (w_out*h_in > h_out*w_in)
		{
			disph = h_out;
			dispw = (h_out*w_in)/h_in;
			lb    = 0.5f*(w_out-dispw);
		}
		else
		{
			dispw = w_out;
			disph = (w_out*h_in)/w_in;
			bb    = 0.5f*(h_out-disph);
		}

		return ncc::v4d(lb,bb,dispw,disph);

}

ncc::v4d ncc::fitToCanvasGL(double w_in, double h_in, double w_out, double h_out) {

	ncc::v4d out = fitToCanvas(w_in, h_in, w_out, h_out);

	out[0] = -1.0+2.0*out[0]/w_out;
	out[1] = -1.0+2.0*out[1]/h_out;
	out[2] = 2.0*out[2]/w_out;
	out[3] = 2.0*out[3]/h_out;

	return out;
}

ncc::RTd ncc::lookAt(ncc::v3d eye,ncc::v3d center,ncc::v3d up) {

	ncc::RTd out;

	ncc::v3d F	= center - eye;
	ncc::v3d f	= F.normalize();
	ncc::v3d UP = up.normalize();

	ncc::v3d s	= ncc::cross(f,UP).normalize();
	ncc::v3d u	= ncc::cross(s,f);

	out[0][0] =	s[0];
	out[1][0] =	s[1];
	out[2][0] =	s[2];
	out[3][0] =  0.0;

	out[0][1] =	u[0];
	out[1][1] =	u[1];
	out[2][1] =	u[2];
	out[3][1] =  0.0;

	out[0][2] =	-f[0];
	out[1][2] =	-f[1];
	out[2][2] =	-f[2];
	out[3][2] =  0.0;

	out[0][3] =  0.0;
	out[1][3] =  0.0;
	out[2][3] =  0.0;
	out[3][3] =  1.0;

	return out*ncc::RTd::translate(-eye[0],-eye[1],-eye[2]);

}

bool ncc::writeImage(std::string fname, ncImageBuffer<ncc::v3ub> & imbuf, bool flip /*=false*/) {

    Magick::Image img;
    img.read( imbuf.width, imbuf.height, "RGB", Magick::CharPixel, imbuf.data );
    img.depth(8);
    if (flip) img.flip();
    img.write(fname);

	return true;
}

bool ncc::writeImage(std::string fname, ncImageBuffer<ncc::v4ub> & imbuf, bool flip /*=false*/) {

	try {
		Magick::Image img;
		img.read( imbuf.width, imbuf.height, "RGBA", Magick::CharPixel, imbuf.data );
	    img.depth(8);
		if (flip) img.flip();
		img.write(fname);
		return true;
    }
    catch( Magick::WarningCoder &warning )
    {
		cerr << "Coder Warning: " << warning.what() << endl;
    }
    catch( Magick::Warning &warning )
    {
		// Handle any other Magick++ warning.
		cerr << "Warning: " << warning.what() << endl;
    }
    catch( Magick::ErrorFileOpen &error )
    {
		// Process Magick++ file open error
		cerr << "Error: " << error.what() << endl;
    }
	catch( std::exception & error )
	{
		// Process any other exceptions derived from standard C++ exception
		cerr << "Caught C++ STD exception: " << error.what() << endl;
	}
	catch( ... )
	{
		// Process *any* exception (last-ditch effort). There is not a lot
		// you can do here other to retry the operation that failed, or exit
	}
	return false;
}

bool ncc::writeImage(std::string fname, ncImageBuffer<unsigned char> & imbuf, bool flip /*=false*/) {

    Magick::Image img;
    img.read( imbuf.width, imbuf.height, "I", Magick::CharPixel, imbuf.data );
    img.depth(8);
    if (flip) img.flip();
    img.write(fname);

	return true;
}

bool ncc::writeImage(std::string fname, ncImageBuffer<float> & imbuf , float minval /*= 0.0f*/, float maxval /*= 1.0f*/, bool flip /*=false*/) {

    ncImageBuffer<unsigned char> tmp(imbuf.width,imbuf.height);
    for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
        tmp[i] = std::min(255,std::max(0,int(((imbuf.data[i]-minval)*255)/(maxval-minval))));
    bool result = writeImage(fname, tmp, flip);
	return result;
}

bool ncc::writeImage(std::string fname, ncImageBuffer<unsigned short> & imbuf , unsigned short minval /*= 0x0000*/, unsigned short maxval /*= 0xFFFF*/, bool flip /*=false*/) {

    ncImageBuffer<unsigned char> tmp(imbuf.width,imbuf.height);
    for (unsigned int i=0;i<imbuf.width*imbuf.height;++i)
        tmp[i] = std::min(255,std::max(0,((int(imbuf.data[i]-minval)*255)/int(maxval-minval))));
    bool result = writeImage(fname, tmp, flip);
	return result;
}

bool ncc::readImage(std::string fname, ncImageBuffer<ncc::v3ub> & imbuf, int level /*=0*/ , bool flip /*=false*/) {

    Magick::Image image;
    image.read(fname);

    int width   = image.columns();
    int height  = image.rows();
    for (int l=0;l<level;++l)
    {
        width   = (width+1)>>1;
        height  = (height+1)>>1;
    }
    Magick::Geometry geom(width,height,0,0,false,false);

    //image.filterType(Magick::PointFilter);
    image.resize(geom);

    if (flip) image.flip();
    image.getPixels(0,0,image.columns(),image.rows());
    imbuf.resize( image.columns(),image.rows());
    image.write(0,0,image.columns(),image.rows(),"RGB",Magick::CharPixel,&imbuf.data[0].x);
	return true;
}

bool ncc::readImage(std::string fname, ncImageBuffer<ncc::v4ub> & imbuf, int level /*=0*/ , bool flip /*=false*/) {

	Magick::Image image;
    image.read(fname);

    int width   = image.columns();
    int height  = image.rows();
    for (int l=0;l<level;++l)
    {
        width   = (width+1)>>1;
        height  = (height+1)>>1;
    }
    Magick::Geometry geom(width,height,0,0,false,false);

    //image.filterType(Magick::PointFilter);
    image.resize(geom);

    if (flip) image.flip();
    image.getPixels(0,0,image.columns(),image.rows());
    imbuf.resize( image.columns(),image.rows());
    image.write(0,0,image.columns(),image.rows(),"RGBA",Magick::CharPixel,&imbuf.data[0].x);
	return true;
}

bool ncc::readImage(std::string fname, ncImageBuffer<unsigned char> & imbuf, int level /*=0*/ , bool flip /*=false*/) {

	Magick::Image image;
    image.read(fname);

    int width   = image.columns();
    int height  = image.rows();
    for (int l=0;l<level;++l)
    {
        width   = (width+1)>>1;
        height  = (height+1)>>1;
    }
    Magick::Geometry geom(width,height,0,0,false,false);

    //image.filterType(Magick::PointFilter);
    image.resize(geom);

    if (flip) image.flip();
    image.getPixels(0,0,image.columns(),image.rows());
    imbuf.resize( image.columns(),image.rows());
    image.write(0,0,image.columns(),image.rows(),"I",Magick::CharPixel,&imbuf.data[0]);
	return true;
}


#define CLIP(in, out)\
   in = in < 0 ? 0 : in;\
   in = in > 0xFFFF ? 0xFFFF : in;\
   out=in;

void ncc::ClearBorders(unsigned short *rgb, int sx, int sy, int w) {
    int i, j;
    // black edges are added with a width w:
    i = 3 * sx * w - 1;
    j = 3 * sx * sy - 1;
    while (i >= 0) {
        rgb[i--] = 0;
        rgb[j--] = 0;
    }
    i = sx * (sy - 1) * 3 - 1 + w * 3;
    while (i > sx) {
        j = 6 * w;
        while (j > 0) {
            rgb[i--] = 0;
            j--;
        }
        i -= (sx - 2 * w) * 3;
    }
}

void ncc::debayer(const unsigned short *bayer, unsigned short *rgb, int sx, int sy) {
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = 1;
    int start_with_green = 0;

    ClearBorders(rgb, sx, sy, 2);
    rgb += 2 * rgbStep + 6 + 1;
    height -= 4;
    width -= 4;

    /* We begin with a (+1 line,+1 column) offset with respect to bilinear decoding, so start_with_green is the same, but blue is opposite */
    blue = -blue;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const unsigned short *bayerEnd = bayer + width;
        const int bayerStep2 = bayerStep * 2;
        const int bayerStep3 = bayerStep * 3;
        const int bayerStep4 = bayerStep * 4;

        if (start_with_green) {
            /* at green pixel */
            rgb[0] = bayer[bayerStep2 + 2];
            t0 = rgb[0] * 5
                 + ((bayer[bayerStep + 2] + bayer[bayerStep3 + 2]) << 2)
                 - bayer[2]
                 - bayer[bayerStep + 1]
                 - bayer[bayerStep + 3]
                 - bayer[bayerStep3 + 1]
                 - bayer[bayerStep3 + 3]
                 - bayer[bayerStep4 + 2]
                 + ((bayer[bayerStep2] + bayer[bayerStep2 + 4] + 1) >> 1);
            t1 = rgb[0] * 5 +
                 ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 3]) << 2)
                 - bayer[bayerStep2]
                 - bayer[bayerStep + 1]
                 - bayer[bayerStep + 3]
                 - bayer[bayerStep3 + 1]
                 - bayer[bayerStep3 + 3]
                 - bayer[bayerStep2 + 4]
                 + ((bayer[2] + bayer[bayerStep4 + 2] + 1) >> 1);
            t0 = (t0 + 4) >> 3;
            CLIP(t0, rgb[-blue]);
            t1 = (t1 + 4) >> 3;
            CLIP(t1, rgb[blue]);
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* B at B */
                rgb[1] = bayer[bayerStep2 + 2];
                /* R at B */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                     -
                     (((bayer[2] + bayer[bayerStep2] +
                        bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                      2]) * 3 + 1) >> 1)
                     + rgb[1] * 6;
                /* G at B */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2]) << 1)
                     - (bayer[2] + bayer[bayerStep2] +
                        bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                     + (rgb[1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP(t0, rgb[-1]);
                t1 = (t1 + 4) >> 3;
                CLIP(t1, rgb[0]);
                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                     + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                     - bayer[3]
                     - bayer[bayerStep + 2]
                     - bayer[bayerStep + 4]
                     - bayer[bayerStep3 + 2]
                     - bayer[bayerStep3 + 4]
                     - bayer[bayerStep4 + 3]
                     +
                     ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                       1) >> 1);
                t1 = rgb[3] * 5 +
                     ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                     - bayer[bayerStep2 + 1]
                     - bayer[bayerStep + 2]
                     - bayer[bayerStep + 4]
                     - bayer[bayerStep3 + 2]
                     - bayer[bayerStep3 + 4]
                     - bayer[bayerStep2 + 5]
                     + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP(t0, rgb[2]);
                t1 = (t1 + 4) >> 3;
                CLIP(t1, rgb[4]);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* R at R */
                rgb[-1] = bayer[bayerStep2 + 2];
                /* B at R */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                     -
                     (((bayer[2] + bayer[bayerStep2] +
                        bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                      2]) * 3 + 1) >> 1)
                     + rgb[-1] * 6;
                /* G at R */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep * 3 +
                                                     2]) << 1)
                     - (bayer[2] + bayer[bayerStep2] +
                        bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                     + (rgb[-1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP(t0, rgb[1]);
                t1 = (t1 + 4) >> 3;
                CLIP(t1, rgb[0]);

                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                     + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                     - bayer[3]
                     - bayer[bayerStep + 2]
                     - bayer[bayerStep + 4]
                     - bayer[bayerStep3 + 2]
                     - bayer[bayerStep3 + 4]
                     - bayer[bayerStep4 + 3]
                     +
                     ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                       1) >> 1);
                t1 = rgb[3] * 5 +
                     ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                     - bayer[bayerStep2 + 1]
                     - bayer[bayerStep + 2]
                     - bayer[bayerStep + 4]
                     - bayer[bayerStep3 + 2]
                     - bayer[bayerStep3 + 4]
                     - bayer[bayerStep2 + 5]
                     + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP(t0, rgb[4]);
                t1 = (t1 + 4) >> 3;
                CLIP(t1, rgb[2]);
            }
        }

        if (bayer < bayerEnd) {
            /* B at B */
            rgb[blue] = bayer[bayerStep2 + 2];
            /* R at B */
            t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                   bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                 -
                 (((bayer[2] + bayer[bayerStep2] +
                    bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                  2]) * 3 + 1) >> 1)
                 + rgb[blue] * 6;
            /* G at B */
            t1 = (((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                    bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2])) << 1)
                 - (bayer[2] + bayer[bayerStep2] +
                    bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                 + (rgb[blue] << 2);
            t0 = (t0 + 4) >> 3;
            CLIP(t0, rgb[-blue]);
            t1 = (t1 + 4) >> 3;
            CLIP(t1, rgb[0]);
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
}

unsigned int ncc::isHDR(std::string fname) {

 	CPGFImage pgf;

#ifdef _WIN32
	HANDLE fd;
	fd = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	int fd = open(fname.c_str(), O_RDONLY);
#endif

	CPGFFileStream stream(fd);
	pgf.Open(&stream);

	const PGFHeader* header = pgf.GetHeader();
	unsigned int hdr = (header->mode == ImageModeRGB48)  || (header->mode == ImageModeGray16)? 1 : 0;
#ifdef _WIN32
	CloseHandle(fd);
#else
	close(fd);
#endif
    return hdr;
}

unsigned int ncc::isGray(std::string fname) {

 	CPGFImage pgf;

#ifdef _WIN32
	HANDLE fd;
	fd = CreateFile(fname.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	int fd = open(fname.c_str(), O_RDONLY);
#endif

	CPGFFileStream stream(fd);
	pgf.Open(&stream);

	const PGFHeader* header = pgf.GetHeader();
	unsigned int gray = (header->mode == ImageModeGrayScale)  || (header->mode == ImageModeGray16)? 1 : 0;
#ifdef _WIN32
	CloseHandle(fd);
#else
	close(fd);
#endif
    return gray;
}

void ncc::writePLY(std::string fname ,std::vector<ncc::v3f4ub> data) {

    fprintf(stderr,"Writing %s...", fname.c_str());
    FILE* plyfile = fopen(fname.c_str(),"wb");

    unsigned int nrvertex = data.size();
    fprintf(plyfile,"ply\n");
    fprintf(plyfile,"format binary_little_endian 1.0\n");
    fprintf(plyfile,"element vertex %d\n",nrvertex);
    fprintf(plyfile,"property float x\n");
    fprintf(plyfile,"property float y\n");
    fprintf(plyfile,"property float z\n");
    fprintf(plyfile,"property uchar red\n");
    fprintf(plyfile,"property uchar green\n");
    fprintf(plyfile,"property uchar blue\n");
    fprintf(plyfile,"property uchar alpha\n");
    fprintf(plyfile,"element face 0\n");
    fprintf(plyfile,"property list uchar int vertex_indices\n");
    fprintf(plyfile,"end_header\n");
    fwrite((char*)&(data[0].x), sizeof(ncc::v3f4ub), nrvertex,plyfile);
    fclose(plyfile);
    fprintf(stderr,"done\n");
}

