#ifndef __NC_CALIBRATIONSET__
#define __NC_CALIBRATIONSET__

#include <vector>
#include "nccalibration.h"
#include "ncmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

class ncCalibrationSet {

public:
	ncCalibrationSet(std::string filename = "", double near_ = 0.25, double far_ = 0.0);

	bool					readFile(std::string filename_, double near_ = 0.25, double far_ = 0.0, void (*setProgress)(const char*,int,const char*) = 0);

	unsigned int			getNrCameras() const;

    void                    setDepthRange(double near,double far = 0.0);

	const ncCalibration &	operator[](unsigned int index) const;
	      ncCalibration &	operator()(unsigned int index);

	void					operator>>(FILE *& pFile);
	void					operator<<(FILE *& pFile);

	std::string				getFilename();

private:
	std::string						filename;
	std::vector<ncCalibration	>	calib;
	std::vector<ncc::v2i		>	stereopairs;

};


#endif //__NC_CALIBRATIONSET__
