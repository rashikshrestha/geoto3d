#ifndef __NC_POSE_LIST__
#define __NC_POSE_LIST__

#include "ncmath.h"
#include <vector>
#include <fstream>

class ncPoseList {

public:

	ncPoseList(std::string filename = "");

	~ncPoseList();

	void				clear();

	bool				readFile(std::string filename_, void (*setProgress)(const char*,int,const char*) = 0);

	const ncc::RTd &	operator[] (unsigned int index) const;
	      ncc::RTd &	operator() (unsigned int index);

	bool				isValid(unsigned int index) const;

	unsigned int		getMinVid(bool only_valid = false) const;
	unsigned int		getMaxVid(bool only_valid = false) const;

	void				operator>>(FILE *& pFile);
	void				operator<<(FILE *& pFile);

	void			    calcTransform();
	ncc::RTd			getTransform() const;

	void				getBoundingBox(ncc::v3d & pmin , ncc::v3d & pmax);

	std::string			getFilename();

private:
	std::string					filename;

	unsigned int				min_vid;
	unsigned int				max_vid;
	unsigned int				nrposes;

	std::vector< ncc::RTd	>	poses;
	bool*						valid;

	ncc::RTd                    transform;
};

#endif
