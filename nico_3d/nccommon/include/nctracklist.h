#ifndef __NC_TRACKLIST__
#define __NC_TRACKLIST__

#include "nccommon.h"
#include "nctrack.h"
//#include "ncpointmap.h"
#include <algorithm>
#include <limits.h>

class ncTrackList {

public:

	bool							addTracks(std::string path, std::string pattern, unsigned int min_pose = 0, unsigned int max_pose = UINT_MAX, void (*setProgress)(const char*,int,const char*) = 0);
	bool							readTracks(std::string path, std::string pattern, unsigned int min_pose = 0, unsigned int max_pose = UINT_MAX, void (*setProgress)(const char*,int,const char*) = 0);
	//bool							readTracks(std::string path, std::string pattern, const ncCalibrationSet & calset, const ncPoseList & poselist, unsigned int min_pose = 0, unsigned int max_pose = UINT_MAX, void (*setProgress)(const char*,int,const char*) = 0);
	//void							createPointMap(const ncCalibrationSet & calset, const ncPoseList & poselist);

	std::vector<ncc::v3d	>  		getPoints() const;

	unsigned int					getNrPoints() const;

	const std::vector<ncTrack > &	getTracks() const;

	//ncPointMap&						getPointMap();

	void							operator>>(FILE *& pFile);
	void							operator<<(FILE *& pFile);

	const ncTrack &					operator[](unsigned int index) const { return tracks[index];}

private:

	void readTrackFile(const char* filename , unsigned int min_pose, unsigned int max_pose);
	void postProcessTracks();

	std::vector<ncTrack		>	tracks;
	//ncPointMap					pointmap;
};

#endif //__NC_TRACKLIST__
