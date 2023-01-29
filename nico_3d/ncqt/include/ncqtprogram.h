#ifndef __NCQT_PROGRAM_H__
#define __NCQT_PROGRAM_H__

#include "ncqt.h"
#include "ncprogram.h"

class ncQtProgram : public ncProgram {
public:
	ncQtProgram(const char* vert, const char* geom, const char* frag);

};


#endif //__NCQT_PROGRAM_H__