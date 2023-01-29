#include "ncposelist.h"

ncPoseList::ncPoseList(std::string filename) {
	poses.clear();
	valid		= 0;
	nrposes 	= 0;
	min_vid		= 0;
	max_vid		= 0;

	if (!filename.empty())
        readFile(filename);
};

ncPoseList::~ncPoseList() {
	clear();
};

void ncPoseList::clear() {

	poses.clear();
	valid		= 0;
	nrposes 	= 0;
	min_vid		= 0;
	max_vid		= 0;
};

std::string	ncPoseList::getFilename() {
	return filename;
}

bool ncPoseList::readFile(std::string filename_, void (*setProgress)(const char*,int,const char*)) {

	filename = filename_;
	
	if (setProgress)
		setProgress("Reading Poses File",0,filename.c_str());

	clear();

	std::ifstream file(filename.c_str(),std::ios_base::binary);
	file >> std::ws;

	unsigned int id;
	file >> id >> std::ws;

    if (id != 200)
    {
        file.close();
        return false;
    }

	unsigned int version;
	file >> version >> std::ws;

	file >> min_vid >> std::ws;
	file >> max_vid >> std::ws;
	file >> nrposes >> std::ws;

	unsigned int viewrange = max_vid-min_vid+1;

	poses.resize(viewrange);
	if (valid)
		delete[] valid;
	valid = new bool[viewrange];

	for (unsigned int i=0; i<viewrange; ++i) {
		valid[i]=false;
		poses[i].setIdentity();
	}

	unsigned int vid;
	for (unsigned int p=0; p<nrposes; ++p) {

		if (setProgress)
			setProgress("Reading Poses File",(100*p)/nrposes,filename.c_str());

		file >> vid >> std::ws;

		for (unsigned int i = 0; i < 3; ++i)
			for (unsigned int j = 0; j < 3; ++j)
				file >> poses[vid-min_vid][j][i] >> std::ws;

		for (unsigned int i = 0; i < 3; ++i)
			file >> poses[vid-min_vid][3][i] >> std::ws;

		valid[vid-min_vid] = true;
	}

	unsigned int index = 0;
	while ((index<viewrange) && (!valid[index]))
		index++;
	if (index<viewrange) {
		ncc::RTd validpose = poses[index];
		for (unsigned int i=0; i<index; ++i)
			poses[i] = validpose;

		for (unsigned int i=index; i<viewrange; ++i)
			if (valid[i])
				validpose = poses[i];
			else
				poses[i] = validpose;
	}
	file.close();

	calcTransform();

	if (setProgress)
		setProgress("Reading Poses File",100,filename.c_str());

    return true;
}

const ncc::RTd & ncPoseList::operator[] (unsigned int index) const {
	return poses[index-min_vid];
}

ncc::RTd & ncPoseList::operator() (unsigned int index) {
	return poses[index-min_vid];
}

bool ncPoseList::isValid(unsigned int index) const{
	if ((index<min_vid) || (index>max_vid))
		return false;
	else
		return valid[index-min_vid];
}

unsigned int ncPoseList::getMinVid(bool only_valid) const {
    if (only_valid)
    {
        unsigned int vid = min_vid;
        while ((vid<=max_vid) && !isValid(vid))
            vid++;
        return vid;
    }
    else
        return min_vid;
}

unsigned int ncPoseList::getMaxVid(bool only_valid) const {
    if (only_valid)
    {
        unsigned int vid = max_vid;
        while ((vid>min_vid) && !isValid(vid))
            vid--;
        return vid;
    }
    else
        return max_vid;
}

void ncPoseList::operator>>(FILE *& pFile) {

	size_t wp;

	unsigned int viewrange = max_vid-min_vid+1;

	wp = fwrite(&min_vid		, sizeof(unsigned int	) 	, 1 		, pFile );
	wp = fwrite(&max_vid		, sizeof(unsigned int	) 	, 1 		, pFile );
	wp = fwrite(&nrposes		, sizeof(unsigned int	) 	, 1 		, pFile );
	wp = fwrite(&poses[0][0][0]	, sizeof(ncc::RTd		) 	, viewrange	, pFile );
	wp = fwrite(&valid[0]		, sizeof(bool		 	) 	, viewrange	, pFile );
	//wp = fwrite(&pcenter[0]		, sizeof(ncc::v4d		) 	, 1			, pFile );
};

void ncPoseList::operator<<(FILE *& pFile) {

	clear();

	size_t rp;

	rp = fread (&min_vid 	, sizeof(unsigned int) 	, 1 				, pFile );
	rp = fread (&max_vid 	, sizeof(unsigned int) 	, 1 				, pFile );
	rp = fread (&nrposes 	, sizeof(unsigned int) 	, 1 				, pFile );

	unsigned int viewrange = max_vid-min_vid+1;
	poses.resize(viewrange);
	if (valid)
		delete[] valid;
	valid = new bool[viewrange];

	rp = fread (&poses[0][0][0]	, sizeof(ncc::RTd	)	, viewrange	, pFile );
	rp = fread (&valid[0]		, sizeof(bool		)	, viewrange	, pFile );
	//rp = fread (&pcenter[0]		, sizeof(ncc::v4d	) 	, 1			, pFile );
};

void ncPoseList::calcTransform() {

	ncc::v3d pmin,pmax;
	getBoundingBox(pmin,pmax);

	ncc::v3d    pcent_old;
	ncc::v3d    pcent_new;
	pcent_old   = 0.5*(pmin+pmax);

	bool	found   = false;
	double	dist    = 0.0;

	for (unsigned int i=0;i<poses.size();++i)
        if (valid[i])
        {
            double tmp = (poses[i][3].xyz()-pcent_old).norm();

            if (!found)
            {
                pcent_new = poses[i][3].xyz();
                dist  = tmp;
                found = true;
            }
            else
            {
                if (tmp<dist)
                {
                    pcent_new = poses[i][3].xyz();
                    dist  = tmp;
                }
            }
        }

    transform.setIdentity();
	transform[3] = ncc::v4d(pcent_new[0],pcent_new[1],pcent_new[2],1.0);
}

ncc::RTd ncPoseList::getTransform() const {
    return transform;
}

void ncPoseList::getBoundingBox(ncc::v3d & pmin , ncc::v3d & pmax) {

	bool    found   = false;

	pmin = pmax = ncc::v3d(0.0,0.0,0.0);
	for (unsigned int i=0;i<poses.size();++i)
        if (valid[i])
        {
            if (!found)
            {
                pmin = pmax = poses[i][3].xyz();
                found = true;
            }
            else
            {
                pmin = ncc::min(pmin,poses[i][3].xyz());
                pmax = ncc::max(pmax,poses[i][3].xyz());
            }
        }
}
