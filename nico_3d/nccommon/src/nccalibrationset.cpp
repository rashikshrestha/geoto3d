#include "nccalibrationset.h"

#include <string>

ncCalibrationSet::ncCalibrationSet(std::string filename, double near_ , double far_) {
	calib.clear();
	stereopairs.clear();

	if (!filename.empty())
        readFile(filename,near_,far_);
}

std::string	ncCalibrationSet::getFilename() {
	return filename;
}

bool ncCalibrationSet::readFile(std::string filename_, double near_, double far_, void (*setProgress)(const char*,int,const char*)) {

	filename = filename_;

	calib.clear();

	std::ifstream file(filename.c_str(),std::ios_base::binary);

	if (!file.is_open())
		return false;

	unsigned int nrcameras;

	file >> std::ws;
	file >> nrcameras >> std::ws;

	ncc::m4x4d	K;
	ncc::RTd	MV;
	MV.setIdentity();
	ncc::v3d	kappa;
	unsigned int			width;
	unsigned int			height;
	std::string				text;

	for (unsigned int c = 0; c<nrcameras; ++c) {

		if (setProgress)
			setProgress("Reading Camera File",(100*c)/nrcameras,filename.c_str());

		for (unsigned int i = 0; i < 2; ++i)
			for (unsigned int j = 0; j < 3; ++j)
				file >> K[j][i] >> std::ws;

		for (unsigned int j = 0; j < 3; ++j)
			K[j][2] = 0;

		for (unsigned int j = 0; j < 3; ++j)
			file >> K[j][3] >> std::ws;

		for (unsigned int i=0; i<3; ++i)
			file >> kappa[i] >> std::ws;

		for (unsigned int i = 0; i < 3; ++i)
			for (unsigned int j = 0; j < 3; ++j)
				file >> MV[j][i] >> std::ws;

		for (unsigned int i=0; i<3; ++i)
			file >> MV[3][i] >> std::ws;

		file >> width  >> std::ws;
		file >> height >> std::ws;

		file >> text >> std::ws;

		calib.push_back(ncCalibration(K,MV,kappa,width,height));
	}

	unsigned int nrstereopairs;
	file >> nrstereopairs >> std::ws;

	ncc::v2i sp;
	stereopairs.clear();
	for (unsigned int i=0; i<nrstereopairs; ++i)
	{
		file >> sp[0] >> std::ws >> sp[1] >> std::ws;
		stereopairs.push_back(sp);
	}

	file.close();

	setDepthRange(near_,far_);

	if (setProgress)
		setProgress("Reading Camera File",100,"");

	return true;
}

unsigned int ncCalibrationSet::getNrCameras() const {
	return calib.size();
}

void ncCalibrationSet::setDepthRange(double near,double far) {

    for (unsigned int i=0;i<calib.size();++i)
        calib[i].setDepthRange(near,far);
}

const ncCalibration & ncCalibrationSet::operator[] (unsigned int index) const {
	return calib[index];
}

ncCalibration & ncCalibrationSet::operator() (unsigned int index) {
	return calib[index];
}

void ncCalibrationSet::operator>>(FILE *& pFile) {

	size_t wp;

	unsigned int nrcameras = calib.size();
	wp = fwrite(&nrcameras		, sizeof(unsigned int)	, 1				, pFile );
	wp = fwrite(&calib[0]		, sizeof(ncCalibration)	, nrcameras		, pFile );
	unsigned int nrstereopairs = stereopairs.size();
	wp = fwrite(&nrstereopairs	, sizeof(unsigned int)	, 1				, pFile );
	wp = fwrite(&stereopairs[0]	, sizeof(ncc::v2i)		, nrstereopairs	, pFile );
}

void ncCalibrationSet::operator<<(FILE *& pFile) {

	size_t rp;

	unsigned int nrcameras;
	rp = fread(&nrcameras		, sizeof(unsigned int)	, 1				, pFile );
	calib.resize(nrcameras);
	rp = fread(&calib[0]		, sizeof(ncCalibration)	, nrcameras		, pFile );
	unsigned int nrstereopairs;
	rp = fread(&nrstereopairs	, sizeof(unsigned int)	, 1				, pFile );
	stereopairs.resize(nrstereopairs);
	rp = fread(&stereopairs[0]	, sizeof(ncc::v2i)		, nrstereopairs	, pFile );
}
