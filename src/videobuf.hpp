#ifndef VIDEOBUF_HPP_
#define VIDEOBUF_HPP_

#include <iostream>

extern "C" {
#include <va/va.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/hwcontext_vaapi.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
}

class videobuf : public std::basic_streambuf<char> {
private: 
    const char *    filename_;
    bool            is_open_;
    char *          buffer_;
    std::streamsize size_;
    int             index_;

    AVFormatContext * av_format_context_;
    AVCodec         * av_decoder_;
    AVCodecContext  * av_decoder_context_;
    AVBufferRef     * av_device_context_;
    int             av_stream_index_;
    AVPixelFormat   av_pixel_format_;
    AVPacket        av_packet_;
    AVFrame         * av_hwframe_;
    AVFrame         * av_swframe_;

    int prime_fd_;
    VADisplay va_display_;
    VAImage va_image_;

    void create_av_format_context_();
    int get_av_stream_index_();
    void create_av_device_context_();
    void create_va_display_();
    void create_av_decoder_context_();
    void flush_av_decoder_context_();
    void open_av_decoder_();
    void create_av_frames_();
    void create_buffer_();

    std::streamsize av_packet_size_();
    std::streamsize av_hwframe_size_();
    std::streamsize av_swframe_size_();
    std::streamsize copy_av_swframe_(char * s, std::streamsize n);
    AVPacket * next_av_packet_();
    AVFrame * next_av_packet_hwframe_();
    AVFrame * next_av_packet_swframe_();
    AVFrame * next_av_hwframe_();
    AVFrame * next_av_swframe_();

    std::streampos seekbeg_(std::streamoff off, std::ios_base::openmode which);
    std::streampos seekcur_(std::streamoff off, std::ios_base::openmode which);
    std::streampos seekend_(std::streamoff off, std::ios_base::openmode which);
protected:
    std::basic_streambuf<char> * setbuf(char * s, std::streamsize n);
    std::streampos seekpos(std::streampos pos, std::ios_base::openmode which);
    std::streampos seekoff(
        std::streamoff off, 
        std::ios_base::seekdir way, 
        std::ios_base::openmode which
    );

    std::streamsize showmanyc();
    std::streamsize xsgetn(char * s, std::streamsize n);
    int pbackfail();
    int uflow();
    int underflow();
public: 
    videobuf();
    ~videobuf();

    // TODO: change 'open' to allow specifying a hw device
    videobuf * open(
        const char * filename, std::ios_base::openmode which
    );
    bool is_open();
    videobuf * close();

    // TODO: add getfmt() function for extracting video properties
    // format includes the width, height, and fourcc information
    int getfmt();
    int getw();
    int geth();

    std::streamsize in_avail();
    int sgetc();
    int sbumpc();
    int snextc();
    std::streamsize sgetn(char * s, std::streamsize n);
    std::streamsize sgetp(char * s, std::streamsize n);
    std::streamsize sgetf(char * s, std::streamsize n);
    int sgetfd();
    int snextfd();
    int sputbackc(char c);
    int sungetc();
};

#endif
