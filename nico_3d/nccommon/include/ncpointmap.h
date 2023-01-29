#ifndef NCPOINTMAP_H
#define NCPOINTMAP_H

#include <vector>
#include <stdio.h>

#include "nccalibrationset.h"
#include "ncposelist.h"

class ncPointMap
{
public:
    ncPointMap();
	~ncPointMap();

	void allocate(const ncCalibrationSet & calset, const ncPoseList & poselist);
	void addIndex(int pid,int cid, int vid);
	const std::vector<int> & getIndices(int cid, int vid) const;

private:

	void clear();

	std::vector<int>* data;
	int nrcid;
	int nrvid;
	int vid_offset;
};

#endif // NCPOINTMAP_H
