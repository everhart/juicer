#include "videobuf.hpp"

/**
 *  find_av_hw_pixel_format
 *  Finds a pixel format supported by both the given av codec and the user's
 *  graphics hardware.
 *
 *  In order to use a graphics device for hardware acceleration, it must be
 *  capable of handling at least one possible pixel format of the av codec.
 *  For example, a video may be encoded with H.264 to support decoding
 *  according to numerous pixel formats, but often times graphics devices will
 *  only implement a few of these possible formats. VAAPI, for example, uses
 *  its own, specialized pixel format for accelerated decoding. 
**/
static enum AVPixelFormat find_av_hw_pixel_format(const AVCodec * av_decoder) {
    for (int i = 0;; i++) {
        const AVCodecHWConfig * av_hw_config;
        av_hw_config = avcodec_get_hw_config(av_decoder, i);
        if (av_hw_config == nullptr) {
            return AV_PIX_FMT_NONE;
        }

        if ((av_hw_config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) &&
            (av_hw_config->device_type == AV_HWDEVICE_TYPE_VAAPI)
        ) {
            return av_hw_config->pix_fmt;
        }
    }
}

/**
 *  get_av_pixel_format
 *  Gets the pixel format to be used with the av codec context.
**/
static enum AVPixelFormat get_av_pixel_format(
    AVCodecContext              * av_decoder_context,
    const enum AVPixelFormat    * av_pixel_formats
) {
    enum AVPixelFormat av_hw_pixel_format;
    const enum AVPixelFormat * av_pixel_format;

    av_hw_pixel_format = find_av_hw_pixel_format(av_decoder_context->codec);
    if (av_hw_pixel_format == AV_PIX_FMT_NONE) {
        return av_hw_pixel_format;
    }
    
    for (int i; av_pixel_formats[i] != -1; i++) {
        if (av_pixel_formats[i] == av_hw_pixel_format) {
            return av_hw_pixel_format;
        }
    }

    return AV_PIX_FMT_NONE;
}

void videobuf::create_av_format_context_() {
    av_register_all();
    avcodec_register_all();

    int ret = avformat_open_input(
        &av_format_context_, filename_, nullptr, nullptr
    );
    if (ret != 0) {
        throw std::invalid_argument("could not open video");
    }
}

int videobuf::get_av_stream_index_() {
    int ret;

    ret = avformat_find_stream_info(av_format_context_, nullptr);
    if (ret < 0) {
        throw std::system_error();
    }

    ret = av_find_best_stream(
        av_format_context_, AVMEDIA_TYPE_VIDEO, -1, -1, &av_decoder_, 0
    );
    if (ret < 0) {
        throw std::system_error();
    }

    av_stream_index_ = ret;
    return ret;
}

void videobuf::create_av_device_context_() {
    int ret = av_hwdevice_ctx_create(
        &av_device_context_, AV_HWDEVICE_TYPE_VAAPI, nullptr, nullptr, 0
    );
    if (ret < 0) {
        throw std::system_error();
    }
}

/*
void videobuf::create_av_device_frames_context_() {
    int ret = av_hwdevice_ctx_create
}
*/

void videobuf::create_va_display_() {
    auto av_device_context = (AVHWDeviceContext *)av_device_context_->data;
    auto av_va_device_context = (AVVAAPIDeviceContext *)av_device_context->hwctx;
    
    va_display_ = av_va_device_context->display;
}

void videobuf::create_av_decoder_context_() {
    int ret;
    AVStream * av_stream;

    av_decoder_context_ = avcodec_alloc_context3(av_decoder_);
    if (av_decoder_context_== nullptr) {
        throw std::bad_alloc();
    }

    av_stream = av_format_context_->streams[av_stream_index_];
    ret = avcodec_parameters_to_context(
        av_decoder_context_, av_stream->codecpar
    );
    if (ret < 0) {
        throw std::system_error();
    }

    av_decoder_context_->get_format = get_av_pixel_format;
    av_decoder_context_->hw_device_ctx = av_buffer_ref(av_device_context_);
}

void videobuf::flush_av_decoder_context_() {
    AVFrame av_frame;
    int ret = avcodec_send_packet(av_decoder_context_, nullptr);
    if (ret < 0) {
        throw std::system_error();
    }

    for (;;) {
        ret = avcodec_receive_frame(av_decoder_context_, &av_frame);
        if (ret == AVERROR_EOF) {
            break;
        }
    }

    avcodec_flush_buffers(av_decoder_context_);
}

void videobuf::open_av_decoder_() {
    AVDictionary * av_decoder_options = NULL;
    av_dict_set(&av_decoder_options, "preset", "ultrafast", 0);
    av_dict_set(&av_decoder_options, "tune", "zerolatency", 0);

    int ret = avcodec_open2(av_decoder_context_, av_decoder_, &av_decoder_options);
    if (ret < 0) {
        throw std::system_error();
    }
}

void videobuf::create_av_frames_() {
    av_hwframe_ = av_frame_alloc();
    av_swframe_ = av_frame_alloc(); 
    if (av_hwframe_ == nullptr || av_swframe_ == nullptr) {
        throw std::system_error();
    }
}

videobuf * videobuf::open(
    const char * filename, std::ios_base::openmode which
) {
    if (which != std::ios_base::in) {
        throw std::runtime_error("output video buffer is not supported");
    }

    filename_ = filename;

    create_av_format_context_();
    get_av_stream_index_();
    create_av_device_context_();
    create_av_decoder_context_();
    open_av_decoder_();
    create_av_frames_();
    create_va_display_();

    av_pixel_format_ =  find_av_hw_pixel_format(av_decoder_);
    is_open_ = true;

    return this;
}

bool videobuf::is_open() {
    return is_open_;
}

videobuf * videobuf::close() {
    av_frame_free(&av_hwframe_);
    av_frame_free(&av_swframe_);  
    av_buffer_unref(&av_device_context_);
    avcodec_free_context(&av_decoder_context_);
    avformat_close_input(&av_format_context_);

    is_open_ = false;

    return this;
}

videobuf::videobuf() : 
    av_format_context_(nullptr),
    av_decoder_(nullptr),
    av_decoder_context_(nullptr),
    av_hwframe_(nullptr),
    av_swframe_(nullptr),
    filename_(nullptr),
    is_open_(false),
    prime_fd_(-1)
{
    //! intentionally left blank
}

videobuf::~videobuf() {
    if (is_open()) close();
}

/**
 *  create_buffer_()
 *
 *  Allocates an internal buffer for use by certain operations on the managed
 *  video sequence. The size of the allocated buffer is always the size of the
 *  current av frame object's memory buffer. 
**/
void videobuf::create_buffer_() {
    size_ = av_hwframe_size_();
    buffer_ = (char *)malloc(size_);
}

std::basic_streambuf<char> * videobuf::setbuf(char * s, std::streamsize n) {
    buffer_ = s;
    size_ = n;

    return this;
}

std::streamsize videobuf::showmanyc() {
    return size_;
}

int videobuf::getfmt() {
    return 0;
}

int videobuf::getw() {
    return av_decoder_context_->width;
}

int videobuf::geth() {
    return av_decoder_context_->height;
}

std::streamsize videobuf::in_avail() {
    if (gptr() == nullptr || gptr() == egptr()) return showmanyc();
    return egptr() - gptr();
}

int videobuf::uflow() {
    int c = underflow();
    if (c == traits_type::eof()) return c;
    
    gbump(1);

    return c;
}

int videobuf::underflow() {
    AVFrame * av_swframe = next_av_swframe_();
    if (av_swframe == nullptr) {
        return traits_type::eof();
    }

    if (buffer_ == nullptr) {
        create_buffer_();
    }

    copy_av_swframe_(buffer_, size_);
    setg(buffer_, buffer_, egptr());
    index_ -= av_hwframe_size_();

    return traits_type::to_int_type(*gptr());
}

int videobuf::sgetc() {
    if (!gptr() || gptr() == egptr()) {
        return underflow();
    }

    return traits_type::to_int_type(*gptr());
}

int videobuf::sbumpc() {
    int c;
    if (!gptr() || gptr() == egptr()) {
        return uflow();
    }
    
    c = traits_type::to_int_type(*gptr());
    ++index_;
    gbump(1);
    return c;
}

int videobuf::snextc() {
    if (sbumpc() == traits_type::eof()) {
        return traits_type::eof();
    }
 
    return sgetc();
}

std::streamsize videobuf::xsgetn(char * s, std::streamsize n) {
    std::streamsize m;
    
    if (gptr() == nullptr || gptr() == egptr()) {
        if (underflow() == traits_type::eof()) return 0;
    }

    m = gptr() - egptr();
    if (n < m) {
        memcpy(s, gptr(), n);
        index_ += n;

        return n;
    }

    memcpy(s, gptr(), m);
    gbump(m), index_ += m;

    return m + xsgetn(s, n - m);
}

int videobuf::pbackfail() {
    seekpos(index_ - 1, std::ios_base::in);
    return traits_type::to_int_type(*gptr());
}

int videobuf::sungetc() {
    if (!gptr()) {
        copy_av_swframe_(buffer_, size_);
        setg(buffer_, buffer_ + index_, buffer_ + index_);
    }
    else if (gptr() == eback()) {
        return pbackfail();
    }

    gbump(-1);
    --index_;
    return traits_type::to_int_type(*gptr());
}

std::streampos videobuf::seekpos(
    std::streampos pos, std::ios_base::openmode which
) {
    if (which != std::ios_base::in) {
        throw std::runtime_error("output video buffer is not supported");
    }

    if (pos < index_) {
        flush_av_decoder_context_();
    }

    for (;;) {
        if (next_av_swframe_() == nullptr) {
            return traits_type::eof();
        }
        else if (index_ + av_swframe_size_() < pos)  {
            break;
        }
    }

    copy_av_swframe_(buffer_, size_);
    setg(buffer_, buffer_ + (pos - (std::streamoff)index_), egptr());
    index_ = pos;

    return index_;
}

std::streampos videobuf::seekbeg_(
    std::streamoff off, std::ios_base::openmode which
) {
    return (off < 0) ? std::streampos(-1) : seekpos(off, which);
}

std::streampos videobuf::seekcur_(
    std::streamoff off, std::ios_base::openmode which
) {
    return seekbeg_(off + index_, which);
}

std::streampos videobuf::seekend_(
    std::streamoff off, std::ios_base::openmode which
) {
    if (off > 0) return traits_type::eof();

    while (next_av_swframe_() != nullptr);
    off += index_;

    flush_av_decoder_context_();
    return seekbeg_(off, which);
}

std::streampos videobuf::seekoff(
    std::streamoff              off, 
    std::ios_base::seekdir      way, 
    std::ios_base::openmode     which
) {
    switch (way) {
    case std::ios_base::beg:
        return seekbeg_(off, which);

    case std::ios_base::cur:
        return seekcur_(off, which);

    case std::ios_base::end:
        return seekend_(off, which);
    
    default:
        return traits_type::eof();
    }
}

/**
 *  av_packet_size_
 *  Returns the buffer size of the current av packet. 
**/
std::streamsize videobuf::av_packet_size_() {
    return av_packet_.size;
}

/**
 *  av_hwframe_size_
 *  Returns the buffer size of the current av hardware frame. 
**/
std::streamsize videobuf::av_hwframe_size_() {
    return av_image_get_buffer_size(
        (AVPixelFormat)av_hwframe_->format, av_hwframe_->width, av_hwframe_->height, 1
    ); 
}

/**
 *  av_swframe_size_
 *  Returns the buffer size of the current av software frame. 
**/
std::streamsize videobuf::av_swframe_size_() {
    return av_image_get_buffer_size(
        (AVPixelFormat)av_swframe_->format, av_hwframe_->width, av_hwframe_->height, 1
    ); 
}

/**
 *  copy_av_frame_
 *  Copies the current av software frame to the given buffer.
**/
std::streamsize videobuf::copy_av_swframe_(char * s, std::streamsize n) {
    int ret;
    std::streamsize size;
    uint8_t * u = (uint8_t *)s;


    if (av_swframe_ == nullptr || av_swframe_->data == nullptr) {
        return 0;
    }

    size = av_swframe_size_();
    if (size < n) {
        n = size;
    }
    ret = av_image_copy_to_buffer(
        u, 
        n,
        av_swframe_->data,
        (const int *)av_swframe_->linesize, 
        (AVPixelFormat)av_swframe_->format,
        av_swframe_->width, 
        av_swframe_->height,
        1
    );
    if (ret < 0) {
        throw std::runtime_error("could not copy av frame");
    }

    return n;
}

/**
 *  next_av_packet_() 
 *  Gets the next av packet available for the given video.
**/
AVPacket * videobuf::next_av_packet_() {
    int ret;

    // when necessary, advance to the next av packet
    if (av_hwframe_ != nullptr) {
        while (next_av_packet_hwframe_() != nullptr);
    }

    // av_packet_unref(&av_packet_);

    for (;;) {
        ret = av_read_frame(av_format_context_, &av_packet_);
        if (ret < 0) {
            av_packet_.data = nullptr;
            return nullptr;
        }
        if (av_stream_index_ == av_packet_.stream_index) {
            break;
        }
    }

    ret = avcodec_send_packet(av_decoder_context_, &av_packet_);
    if (ret < 0) {
        throw std::runtime_error("could not send av packet");
    }

    return &av_packet_;
}

/**
 *  next_av_packet_hwframe_
 *
 *  Gets the next available av hardware frame from the current av packet of the
 *  managed video sequence. If no remaining av frames are available for the
 *  current av packet, or if no current av packet exists, then nullptr is
 *  returned. 
**/
AVFrame * videobuf::next_av_packet_hwframe_() {
    int ret = avcodec_receive_frame(av_decoder_context_, av_hwframe_);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        return nullptr;
    }
    else if (ret < 0) {
        throw std::runtime_error("could not recieve av frame");
    }


    if (gptr() != nullptr) index_ += gptr() - egptr();
    index_ += av_hwframe_size_();
    
    return av_hwframe_;
}

/**
 *  next_av_packet_swframe_
 *
 *  Gets the next available av software frame from the current av packet of the
 *  managed video sequence. If no remaining av frames are available for the
 *  current av packet, or if no current av packet exists, then nullptr is
 *  returned. 
 *
 *  Internally, this function is equivelant to calling
 *  @next_av_packet_hwframe_, memory mapping the resultant device buffer, and
 *  then using this map to perform a device to host memory transfer.
**/
AVFrame * videobuf::next_av_packet_swframe_() {
    if (next_av_packet_hwframe_() == nullptr) {
        return nullptr;
    }
    std::cout << "test 3\n";

    if (av_hwframe_->format == av_pixel_format_) {
        int ret = av_hwframe_transfer_data(av_swframe_, av_hwframe_, 0);
        if (ret < 0) {
            throw std::system_error();
        }
        return av_swframe_;
    }

    return av_hwframe_;
}

/**
 *  next_av_hwframe_
 *  Gets the next available av frame in the managed video sequence, and
 *  advances the sequence. A new av packet is obtained whenever necessary. 
 *
 *  The av_frame_ that is recieved is stored in the class member @av_frame_.
 *  If an av_packet_ is recieved, that packet is stored in the class member
 *  @av_packet_.
**/
AVFrame * videobuf::next_av_hwframe_() {
    if (next_av_packet_hwframe_() != nullptr) {
        return av_hwframe_;
    }

    next_av_packet_();
    return next_av_packet_hwframe_();
}

/**
 *  next_av_swframe_
 *  Gets the next available av software frame in the managed video sequence,
 *  and advances the sequence. A new av packet is obtained whenever necessary. 
 *
 *  The av software frame that is recieved is stored in the class member
 *  @av_swframe_.  If an av_packet_ is recieved, that packet is stored in the
 *  class member @av_packet_.
**/
AVFrame * videobuf::next_av_swframe_() {
    if (next_av_packet_swframe_() != nullptr) {
        return av_swframe_;
    }

    next_av_packet_();
    return next_av_packet_swframe_();
}

/**
 *  sgetp
 *  Extracts, as raw data, the next packet from the managed input video
 *  sequence.
 *
 *  @param s  The buffer to place the packet contents.
 *  @param n  The size, in bytes, of the buffer.
 *
 *  If the buffer is smaller than the size of the packet, only the amount of
 *  raw data that can fit in the buffer is placed. However, the input sequence
 *  is still advanced by one packet worth of data.
**/
std::streamsize videobuf::sgetp(char * s, std::streamsize n) {
    std::streamsize sz = 0;

    if (next_av_packet_() == nullptr) { 
        return 0;
    }

    while (next_av_packet_swframe_() != nullptr) {
        sz += copy_av_swframe_(s, n);
    } 

    return sz;
}

/**
 *  sgetf
 *  Extracts, as raw data, the next frame from the managed input video
 *  sequence.
 *
 *  @param s  The buffer to place the frame contents.
 *  @param n  The size, in bytes, of the buffer.
 *
 *  If the buffer is smaller than the size of the frame, only the amount of raw
 *  data that can fit in the buffer is placed. However, the input sequence is
 *  still advanced by one frame worth of data.  
**/
std::streamsize videobuf::sgetf(char * s, std::streamsize n) {
    AVFrame * av_swframe = next_av_swframe_();
    return (av_swframe == nullptr) ? 0 : copy_av_swframe_(s, n);
}

int videobuf::sgetfd() {
    VAStatus va_result;
    VASurfaceID va_surface_id = (VASurfaceID)(uintptr_t)av_hwframe_->data[3];
    VABufferInfo va_buffer_info;

    vaSyncSurface(va_display_, va_surface_id);
    va_result = vaDeriveImage(va_display_, va_surface_id, &va_image_);
    if (va_result != VA_STATUS_SUCCESS) {
        throw std::runtime_error("could not derive va image");
    }

    va_result = vaAcquireBufferHandle(
        va_display_, va_image_.buf, &va_buffer_info
    );
    if (va_result != VA_STATUS_SUCCESS) {
        throw std::runtime_error("could not get prime handle");
    }

    prime_fd_ = va_buffer_info.handle;
    return prime_fd_;
}

int videobuf::snextfd() {
    auto temp = (AVHWFramesContext *)av_decoder_context_->hw_frames_ctx->data;
    std::cout << "format = " 
              << temp->sw_format
              << '|' 
              << AV_PIX_FMT_NV12
              << std::endl;
    std::cout << "wxh = " << av_hwframe_->width << "x" << av_hwframe_->height << std::endl;
    if (prime_fd_ >= 0) {
        VAStatus va_result = vaReleaseBufferHandle(va_display_, va_image_.buf);
        if (va_result != VA_STATUS_SUCCESS) {
            throw std::runtime_error("could not release prime handle");
        }
        
        va_result = vaDestroyImage(va_display_, va_image_.image_id);
        if (va_result != VA_STATUS_SUCCESS) {
            throw std::runtime_error("could not destroy va image");
        }
    }

    return (next_av_hwframe_() == nullptr) ? -1 : sgetfd();
}
