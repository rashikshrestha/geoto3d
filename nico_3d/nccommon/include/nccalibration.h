#ifndef NCCALIBRATION_H
#define NCCALIBRATION_H

#include <stdio.h>
#include "ncmath.h"

#ifdef WIN32
#include "windows.h"
#undef near
#undef far
#endif

class ncCalibration
{
public:
    ncCalibration() {

        Kmat.setIdentity();
        MVmat.setIdentity();
        w = h = 256;
        vignet 	= 0.0f;
        gamma 	= 1.0f;
        scale 	= 1.0f;
        wb 		= ncc::v3f(2.0f,1.0f,2.0f);

    };

	ncCalibration(const ncCalibration & cal) {
		Kmat	= cal.Kmat;
		MVmat	= cal.MVmat;
		kappavec= cal.kappavec;
		w 		= cal.w;
		h 		= cal.h;
        vignet 	= cal.vignet;
        gamma 	= cal.gamma;
        scale 	= cal.scale;
        wb 		= cal.wb;
	}

	ncCalibration(ncc::m4x4d & K_, ncc::RTd & MV_, ncc::v3d & kappa_, unsigned int & width_, unsigned int & height_) {

		Kmat       	= K_;

		double near	= 1.0;
		Kmat[0][2] 	= 0.0;
		Kmat[1][2] 	= 0.0;
		Kmat[2][2] 	= 1.0;
		Kmat[3][2] 	= -near;

		MVmat      	= MV_;
		kappavec   	= kappa_;

		w           = width_;
		h           = height_;
        vignet 		= 0.0f;
        gamma 		= 1.0f;
	    scale 		= 1.0f;
        wb 			= ncc::v3f(1.0f,1.0f,1.0f);
	}

	void setDepthRange(double near,double far = 0.0) {

		if (far == 0.0) {
			Kmat[0][2] 	= 0.0;
			Kmat[1][2] 	= 0.0;
			Kmat[2][2] 	= 1.0;
			Kmat[3][2] 	= -near;
		} else {
			Kmat[0][2] 	= 0.0;
			Kmat[1][2] 	= 0.0;
			Kmat[2][2] 	= far/(far-near);
			Kmat[3][2] 	= -far*near/(far-near);
		}
	}

	void getDepthRange(double & near,double & far) const {

        if (Kmat[2][2] 	== 1.0)
        {
            near = -Kmat[3][2];
            far = 0.0;
        }
        else
        {
            near = -Kmat[3][2]/Kmat[2][2];
            far  = -Kmat[3][2]/(Kmat[2][2]-1.0);
        }
	}

	void setVignet(float vignet_) {
		vignet = vignet_;
	}

	float getVignet() const {
		return vignet;
	}

	void setGamma(float gamma_) {
		gamma = gamma_;
	}

	float getGamma() const {
		return gamma;
	}

	void setScale(float scale_) {
		scale = scale_;
	}

	float getScale() const {
		return scale;
	}

	void setWB(ncc::v3f wb_) {
		wb = wb_;
	}

	ncc::v3f getWB() const {
		return wb;
	}

	void setK(ncc::m4x4d K) {
		Kmat = K;
	}

	void setMV(ncc::RTd MV) {
		MVmat = MV;
	}

	void setKappa(ncc::v3d kappa) {
		kappavec = kappa;
	}

	ncc::m4x4d K() const {
		return Kmat;
	}

	ncc::m4x4d KGL(int lod) const {
		return ncc::PtoPGL(Kmat,width(lod)<<lod,height(lod)<<lod);
	}

	ncc::m4x4d Ktex(int lod) const {
		return ncc::PtoPTEX(Kmat,width(lod)<<lod,height(lod)<<lod);
	}

	ncc::RTd MV() const {
		return MVmat;
	}

	ncc::v3d camPos() const {
		return MVmat[3].xyz();
	}

	unsigned int width(int lod) const {
		return (w>>lod);
	}

	unsigned int height(int lod) const {
		return (h>>lod);
	}

	ncc::v4i VP(int lod) const {
		return ncc::v4i(0,0,width(lod),height(lod));
	}

	ncc::v3d kappa() const {
		return kappavec;
	}

	void print(const char* text = 0) {
		if (text)
			fprintf(stderr,"%s\n",text);
		K().print("K");
		KGL(0).print("KGL(0)");
		Ktex(0).print("Ktex(0)");
		MV().print("MV");
		kappa().print("kappa");
		fprintf(stderr,"%dx%d\n",width(0),height(0));
	}

    void setImageSize(int w_, int h_ ) {
        w = w_;
        h = h_;
    }

	int getDiscreteCameraDirection() {

		ncc::v3f dir = (MV()[2]).xyz().normalize();

		float angle 	= atan2(dir.y,dir.x);

		ncc::v2d dir1(cos(angle),sin(angle));

		float value = 0.0f;
		float index = 0;

		for (int i=0;i<8;++i)
		{
			float angle2 = 0.25*M_PI*i;
			ncc::v2d dir2(cos(angle2),sin(angle2));
			float tmp = dir1.x*dir2.x + dir1.y*dir2.y;
			if (tmp>value)
			{
				value = tmp;
				index = i;
			}
		}

		return index;
	}

protected:

	ncc::m4x4d		Kmat;

	ncc::RTd    	MVmat;
    ncc::v3d 		kappavec;

    unsigned int 	w;
    unsigned int 	h;

    float 			vignet;
    float 			gamma;
    float 			scale;
    ncc::v3f		wb;
};

#endif // NCCALIBRATION_H
