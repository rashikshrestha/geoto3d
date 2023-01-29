#include "nctrack.h"

#include <algorithm>
#include <stdio.h>

// ncTrackView

ncTrackView::ncTrackView() {};

ncTrackView::ncTrackView(unsigned int vid_, unsigned int cid_) {
    vid = vid_;
    cid = cid_;
}

bool ncTrackView::operator<(const ncTrackView & tv) const {

    if (vid!=tv.vid)
        return(vid<tv.vid);
    else
        return(cid<tv.cid);
}

bool ncTrackView::operator==(const ncTrackView & tv) const {
    return ((vid==tv.vid) && (cid==tv.cid));
}


// ncTrack

ncTrack::ncTrack() {};

ncTrack::ncTrack(const ncc::v3d & p)
{
	point = p;
    views.clear();
}

void ncTrack::addView(const ncTrackView tv) {
    views.push_back(tv);
}

bool ncTrack::operator<(const ncTrack & t) const {

	for (int i=0;i<3;++i)
		if (point[i]!=t.point[i])
			return (point[i]<t.point[i]);
	return false;
}

bool ncTrack::operator==(const ncTrack & t) const {
	return (point==t.point);
}

void ncTrack::sort() {
    std::sort(views.begin(),views.end());
}

void ncTrack::merge(const ncTrack & t) {

    views.insert(views.end(), t.views.begin(), t.views.end());

    std::vector<ncTrackView>::iterator it;
    std::sort(views.begin(),views.end());
    it = std::unique(views.begin(),views.end());
    views.resize(it-views.begin());
}

void ncTrack::operator>>(FILE *& pFile) {

	size_t wp;

	wp = fwrite(&point[0] 	, sizeof(ncc::v3d		) 	, 1 		, pFile );
	unsigned int nrviews = views.size();
	wp = fwrite(&nrviews 	, sizeof(unsigned int	) 	, 1 		, pFile );
	if (nrviews>0)
		wp = fwrite(&views[0] 	, sizeof(ncTrackView) 	, nrviews	, pFile );
}

void ncTrack::operator<<(FILE *& pFile) {

	size_t rp;

	rp = fread(&point[0]		, sizeof(ncc::v3d	) 	, 1 		, pFile );
	unsigned int nrviews;
	rp = fread(&nrviews 	, sizeof(unsigned int	) 	, 1 		, pFile );
	views.resize(nrviews);
	if (nrviews>0)
		rp = fread(&views[0] 	, sizeof(ncTrackView) 	, nrviews	, pFile );
}
