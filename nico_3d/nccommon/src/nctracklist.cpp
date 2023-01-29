#include "nctracklist.h"

bool ncTrackList::addTracks(std::string path, std::string pattern, unsigned int min_pose /*= UINT_MIN*/, unsigned int max_pose /*= UINT_MAX*/, void (*setProgress)(const char*,int,const char*) /*= 0*/) {

	std::vector<std::string> filenames = ncc::getFiles(path, pattern);

	for (unsigned int i=0; i<filenames.size(); ++i)
	{
		if (setProgress)
			setProgress("Reading Tracks",(100*i)/filenames.size(),filenames[i].c_str());
		readTrackFile(filenames[i].c_str(),min_pose,max_pose);
	}
	if (setProgress)
		setProgress("Reading Tracks",100,"");

	postProcessTracks();

	return (filenames.size()>0);
}

bool ncTrackList::readTracks(std::string path, std::string pattern, unsigned int min_pose /*= UINT_MIN*/, unsigned int max_pose /*= UINT_MAX*/, void (*setProgress)(const char*,int,const char*) /*= 0*/) {
	tracks.clear();
	return addTracks(path, pattern, min_pose, max_pose, setProgress);
}

#if 0
bool ncTrackList::readTracks(std::string path, std::string pattern, const ncCalibrationSet & calset, const ncPoseList & poselist, unsigned int min_pose /*= UINT_MIN*/, unsigned int max_pose /*= UINT_MAX*/, void (*setProgress)(const char*,int,const char*) /*= 0*/) {

	bool result = readTracks(path, pattern, min_pose, max_pose, setProgress);
	createPointMap(calset, poselist);
	return result;
}

void ncTrackList::createPointMap(const ncCalibrationSet & calset, const ncPoseList & poselist) {

	pointmap.allocate(calset,poselist);
	for (unsigned int i=0;i<tracks.size();++i)
		for (unsigned int j=0;j<tracks[i].views.size();++j)
			pointmap.addIndex(i,tracks[i].views[j].cid,tracks[i].views[j].vid);
}

ncPointMap& ncTrackList::getPointMap() {
	return pointmap;
}

#endif

std::vector<ncc::v3d	>  ncTrackList::getPoints() const {
	std::vector<ncc::v3d	> out;
	out.resize(tracks.size());
	for (unsigned int i=0;i<tracks.size();++i)
		out[i] = tracks[i].point;
	return out;
}

unsigned int ncTrackList::getNrPoints() const {
	return tracks.size();
}

const std::vector<ncTrack > & ncTrackList::getTracks() const {
	return tracks;
}

void ncTrackList::operator>>(FILE *& pFile) {

	size_t wp;
	unsigned int nrtracks = tracks.size();

	wp = fwrite(&nrtracks 	, sizeof(unsigned int) 	, 1 		, pFile );
	for (unsigned int i=0;i<tracks.size();++i)
		tracks[i] >> pFile;
}

void ncTrackList::operator<<(FILE *& pFile) {

	size_t rp;
	unsigned int nrtracks;
	rp = fread(&nrtracks 	, sizeof(unsigned int) 	, 1 		, pFile );
	tracks.resize(nrtracks);
	for (unsigned int i=0;i<tracks.size();++i)
		tracks[i] << pFile;

}

void ncTrackList::readTrackFile(const char* filename , unsigned int min_pose, unsigned int max_pose) {

	std::ifstream file(filename, std::ios::in | std::ios::binary);

	unsigned int id;
	unsigned int version;
	unsigned int min_view, max_view;
	unsigned int ntracks;

	file.read((char *)&id		, sizeof(unsigned int));
	file.read((char *)&version	, sizeof(unsigned int));
	file.read((char *)&min_view	, sizeof(unsigned int));
	file.read((char *)&max_view	, sizeof(unsigned int));

	if ((max_view >= min_pose) && (min_view <= max_pose))
	{
		file.read((char *)&ntracks, sizeof(unsigned int));

#if 0
		fprintf(stderr,"id = %d\n",id);
		fprintf(stderr,"version = %d\n",version);
		fprintf(stderr,"min_view = %d\n",min_view);
		fprintf(stderr,"max_view = %d\n",max_view);
		fprintf(stderr,"ntracks = %d\n",ntracks);
#endif

		ncc::v3d p3d;
		ncc::v2f p2f;
		ncc::v3f cov;
		unsigned int vid;
		unsigned int cid;

		for (unsigned int i = 0; i < ntracks; ++i) {

			file.read((char *)&p3d[0], sizeof(double));
			file.read((char *)&p3d[1], sizeof(double));
			file.read((char *)&p3d[2], sizeof(double));

			// read measurements
			unsigned int nentries;
			file.read((char *)&nentries, sizeof(unsigned int));

			ncTrack track(p3d);

			for (unsigned int j = 0; j < nentries; ++j) {

				// read view index
				file.read((char *)&vid, sizeof(unsigned int));

				// read camera index
				file.read((char *)&cid, sizeof(unsigned int));
				//fprintf(stderr,"vid = %d\n",vid);
				//fprintf(stderr,"cid = %d\n",cid);

				// read point (2d)
				file.read((char *)&p2f[0], sizeof(float));
				file.read((char *)&p2f[1], sizeof(float));

				// read inverse covariance (2d)
				file.read((char *)&cov[0], sizeof(float));
				file.read((char *)&cov[1], sizeof(float));
				file.read((char *)&cov[2], sizeof(float));

				//if ((cid==4) || (cid==5))
					track.addView(ncTrackView(vid,cid));
			}

			if (track.views.size()>0)
				tracks.push_back(track);
		}
	}
	file.close();
}

void ncTrackList::postProcessTracks() {

	std::vector<ncTrack>::iterator it;
	std::sort(tracks.begin(),tracks.end());

	bool* valid = new bool[tracks.size()];

	for (unsigned int i=0;i<tracks.size();++i)
	{
		valid[i]=true;
		tracks[i].sort();
	}

	for (int i=tracks.size()-1;i>0;--i)
	{
		if (tracks[i]==tracks[i-1]) {
			tracks[i-1].merge(tracks[i]);
			valid[i]=false;
		}
	}

	int count = 0;
	for (unsigned int i=0;i<tracks.size();++i)
		if (valid[i])
			tracks[count++]=tracks[i];

	tracks.resize(count);

	delete[] valid;
}

