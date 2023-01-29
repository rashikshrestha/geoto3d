#include "nccodec.h"

int ncCodec::start(const char* filename, int w, int h, int br, int fps) {

    int ret;

    av_register_all();

    printf("Encode video file %s\n", filename);
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    c->bit_rate			= br;
    c->width			= w;
    c->height			= h;
    c->time_base.num    = 1;
	c->time_base.den    = fps;
    c->gop_size			= 10;
    c->max_b_frames		= 1;
    c->pix_fmt			= AV_PIX_FMT_YUV420P;

    if(codec_id == AV_CODEC_ID_H264)
    {
        av_opt_set(c->priv_data, "preset", "slow", 0);
        av_opt_set_int(c->priv_data, "crf", 22, 0);
    }
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height, c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        exit(1);
    }

    rgba_frame      = alloc_picture(PIX_FMT_BGRA, rgba_buf,c->width, c->height);
    img_convert_ctx = sws_getCachedContext(0 , c->width, c->height, PIX_FMT_RGBA, c->width, c->height, c->pix_fmt, SWS_LANCZOS, NULL, NULL, NULL);

    frame_index = 0;

    return 0;
}

AVFrame* ncCodec::alloc_picture(enum PixelFormat pix_fmt, uint8_t* & buf,int width, int height) {
    AVFrame *pict;
    int size;

    pict = avcodec_alloc_frame();
    if (!pict)
        return NULL;
    size = avpicture_get_size(pix_fmt, width, height);
    buf = (uint8_t*)av_malloc(size);
    if (!buf) {
        av_free(pict);
        return NULL;
    }
    avpicture_fill((AVPicture *)pict, buf,
                   pix_fmt, width, height);

    return pict;
}

int ncCodec::addFrame(bool flip) {

    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;
    fflush(stdout);

    if (flip)
    {
        int offset = c->width*4*(c->height-1);
        int stride = -c->width*4;
        uint8_t* flipdata[4]    = { rgba_frame->data[0]+offset, rgba_frame->data[1]+offset, rgba_frame->data[2]+offset, rgba_frame->data[3]+offset };
        int      flipstride[4]  = { stride,stride,stride,stride };
        sws_scale(img_convert_ctx, flipdata, flipstride, 0, c->height, frame->data, frame->linesize);
    }
    else
        sws_scale(img_convert_ctx, rgba_frame->data, rgba_frame->linesize, 0, c->height, frame->data, frame->linesize);

    frame->pts = frame_index;

    int got_output;
    int ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
    if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        exit(1);
    }
    if (got_output) {
        //printf("Write frame %3d (size=%5d)\n", frame_index, pkt.size);
        fwrite(pkt.data, 1, pkt.size, f);
        av_free_packet(&pkt);
    }

    frame_index++;

    return 0;
}

int ncCodec::finish() {

    int got_output;
    for (got_output = 1; got_output; frame_index++) {
        fflush(stdout);
        int ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            exit(1);
        }
        if (got_output) {
            //printf("Write frame %3d (size=%5d)\n", frame_index, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }
    }
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);
    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    avcodec_free_frame(&frame);

    av_free(rgba_frame->data[0]);
    av_free(rgba_frame);

    printf("\n");

    return 0;
}

uint8_t*  ncCodec::getVideoBuffer() {
    return rgba_buf;
}
