#include "ncqtprogram.h"

ncQtProgram::ncQtProgram(const char* vert, const char* geom, const char* frag) {

    //fprintf(stderr,"ncQtProgram( %s , %s , %s)\n",vert,geom,frag);

    std::vector<std::string> vfiles,gfiles,ffiles,vsrc,gsrc,fsrc;
    if (vert)
        vfiles = ncc::split(std::string(vert), ',');
    if (geom)
        gfiles = ncc::split(std::string(geom), ',');
    if (frag)
        ffiles = ncc::split(std::string(frag), ',');

	for (unsigned int i=0;i<vfiles.size();++i)
	{
        QFile vfile(vfiles[i].c_str());
        vfile.open(QIODevice::ReadOnly);
        vsrc.push_back( std::string(vfile.readAll().data()) );
        vfile.close();
	}

    for (unsigned int i=0;i<gfiles.size();++i)
	{
        QFile gfile(gfiles[i].c_str());
        gfile.open(QIODevice::ReadOnly);
        gsrc.push_back( std::string(gfile.readAll().data()) );
        gfile.close();
	}

	for (unsigned int i=0;i<ffiles.size();++i)
	{
	    QFile ffile(ffiles[i].c_str());
        ffile.open(QIODevice::ReadOnly);
        fsrc.push_back( std::string(ffile.readAll().data()) );
        ffile.close();
	}

	loadSources(vsrc,gsrc,fsrc);
}
