#ifndef _MASTER_H_
#define _MASTER_H_

#define ANSI_DECLARATORS
#define SINGLE
#define NO_TIMER
#define REDUCED
#define CDT_ONLY
#define TRILIBRARY

//#ifdef SINGLE
//#define REAL float
//#else /* not SINGLE */
#define REAL double
//#endif /* not SINGLE */

// ** gltx **
// Return codes
#define NORMAL_EXIT 0
#define OUT_OF_MEMORY 1
#define NOT_ENOUGH_PARAMS 2
#define INVALID_FILE 3
#define INVALID_SUBSAMPLING 4

#endif // _MASTER_H_
