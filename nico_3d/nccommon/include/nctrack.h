#ifndef NCTRACK_H
#define NCTRACK_H

#include "ncmath.h"
#include <vector>

class ncTrackView {

public:
    ncTrackView();
	ncTrackView(unsigned int vid_, unsigned int cid_);
    bool operator<(const ncTrackView & tv) const;
    bool operator==(const ncTrackView & tv) const;

public:
    unsigned int vid;
    unsigned int cid;
};

class ncTrack
{
public:
    ncTrack();
	ncTrack(const ncc::v3d & p);
    void addView(const ncTrackView tv);
    bool operator<(const ncTrack & t) const;
    bool operator==(const ncTrack & t) const;
    void sort();
    void merge(const ncTrack & t);

	void operator>>(FILE *& pFile);
	void operator<<(FILE *& pFile);

public:
	ncc::v3d					point;
	std::vector<ncTrackView>    views;
};

#endif // NCTRACK_H
