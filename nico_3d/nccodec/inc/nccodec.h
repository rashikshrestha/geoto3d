#ifndef __NC_CODEC_H__
#define __NC_CODEC_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include "stdint.h"
#define snprintf _snprintf
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/mathematics.h"
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>

#ifdef __cplusplus
}
#endif

class ncCodec {

public:
    int         start(const char* fname, int w, int h, int br, int fps);
    int         addFrame(bool flip = 0);
    int         finish();
    uint8_t*    getVideoBuffer();

private:
    AVFrame*    alloc_picture(enum PixelFormat pix_fmt, uint8_t* & buf,int width, int height);

    int             codec_id;
    AVCodec         *codec;
    AVCodecContext  *c;
    FILE            *f;
    AVFrame         *frame;
    AVPacket        pkt;
    int             frame_index;

    uint8_t         *rgba_buf;
    AVFrame         *rgba_frame;
    SwsContext      *img_convert_ctx;
};

#endif
