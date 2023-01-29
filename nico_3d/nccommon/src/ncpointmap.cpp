#include "ncpointmap.h"

ncPointMap::ncPointMap()
{
	data = 0;
}

ncPointMap::~ncPointMap()
{
	clear();
}

void ncPointMap::clear()
{
	if (data) {
		delete[] data;
		data = 0;
	}
}

void ncPointMap::allocate(const ncCalibrationSet & calset, const ncPoseList & poselist) {

	clear();
	vid_offset	= poselist.getMinVid();
	nrvid		= poselist.getMaxVid()-poselist.getMinVid()+1;
	nrcid		= calset.getNrCameras();
	data		= new std::vector<int>[nrvid*nrcid];
}

void ncPointMap::addIndex(int pid,int cid, int vid) {
	data[(vid-vid_offset)*nrcid+cid].push_back(pid);
}

const std::vector<int> & ncPointMap::getIndices(int cid, int vid) const {
	return data[(vid-vid_offset)*nrcid+cid];
}
