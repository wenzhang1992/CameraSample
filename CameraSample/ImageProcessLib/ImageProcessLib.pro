#-------------------------------------------------
#
# Project created by QtCreator 2019-05-25T21:50:18
#
#-------------------------------------------------

QT       -= gui

TARGET = ImageProcessLib
TEMPLATE = lib

DEFINES += IMAGEPROCESSLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug,debug|release){
    DESTDIR =$$PWD/../../xdebug
}

SOURCES += \
        ImageProcessLib.cpp

HEADERS += \
        ImageProcessLib.h \
        imageprocesslib_global.h \ 
    Include/libavcodec/ac3_parser.h \
    Include/libavcodec/adts_parser.h \
    Include/libavcodec/avcodec.h \
    Include/libavcodec/avdct.h \
    Include/libavcodec/avfft.h \
    Include/libavcodec/d3d11va.h \
    Include/libavcodec/dirac.h \
    Include/libavcodec/dv_profile.h \
    Include/libavcodec/dxva2.h \
    Include/libavcodec/jni.h \
    Include/libavcodec/mediacodec.h \
    Include/libavcodec/qsv.h \
    Include/libavcodec/vaapi.h \
    Include/libavcodec/vdpau.h \
    Include/libavcodec/version.h \
    Include/libavcodec/videotoolbox.h \
    Include/libavcodec/vorbis_parser.h \
    Include/libavcodec/xvmc.h \
    Include/libavdevice/avdevice.h \
    Include/libavdevice/version.h \
    Include/libavfilter/avfilter.h \
    Include/libavfilter/buffersink.h \
    Include/libavfilter/buffersrc.h \
    Include/libavfilter/version.h \
    Include/libavformat/avformat.h \
    Include/libavformat/avio.h \
    Include/libavformat/version.h \
    Include/libavutil/adler32.h \
    Include/libavutil/aes.h \
    Include/libavutil/aes_ctr.h \
    Include/libavutil/attributes.h \
    Include/libavutil/audio_fifo.h \
    Include/libavutil/avassert.h \
    Include/libavutil/avconfig.h \
    Include/libavutil/avstring.h \
    Include/libavutil/avutil.h \
    Include/libavutil/base64.h \
    Include/libavutil/blowfish.h \
    Include/libavutil/bprint.h \
    Include/libavutil/bswap.h \
    Include/libavutil/buffer.h \
    Include/libavutil/camellia.h \
    Include/libavutil/cast5.h \
    Include/libavutil/channel_layout.h \
    Include/libavutil/common.h \
    Include/libavutil/cpu.h \
    Include/libavutil/crc.h \
    Include/libavutil/des.h \
    Include/libavutil/dict.h \
    Include/libavutil/display.h \
    Include/libavutil/downmix_info.h \
    Include/libavutil/encryption_info.h \
    Include/libavutil/error.h \
    Include/libavutil/eval.h \
    Include/libavutil/ffversion.h \
    Include/libavutil/fifo.h \
    Include/libavutil/file.h \
    Include/libavutil/frame.h \
    Include/libavutil/hash.h \
    Include/libavutil/hmac.h \
    Include/libavutil/hwcontext.h \
    Include/libavutil/hwcontext_cuda.h \
    Include/libavutil/hwcontext_d3d11va.h \
    Include/libavutil/hwcontext_drm.h \
    Include/libavutil/hwcontext_dxva2.h \
    Include/libavutil/hwcontext_mediacodec.h \
    Include/libavutil/hwcontext_qsv.h \
    Include/libavutil/hwcontext_vaapi.h \
    Include/libavutil/hwcontext_vdpau.h \
    Include/libavutil/hwcontext_videotoolbox.h \
    Include/libavutil/imgutils.h \
    Include/libavutil/intfloat.h \
    Include/libavutil/intreadwrite.h \
    Include/libavutil/lfg.h \
    Include/libavutil/log.h \
    Include/libavutil/lzo.h \
    Include/libavutil/macros.h \
    Include/libavutil/mastering_display_metadata.h \
    Include/libavutil/mathematics.h \
    Include/libavutil/md5.h \
    Include/libavutil/mem.h \
    Include/libavutil/motion_vector.h \
    Include/libavutil/murmur3.h \
    Include/libavutil/opt.h \
    Include/libavutil/parseutils.h \
    Include/libavutil/pixdesc.h \
    Include/libavutil/pixelutils.h \
    Include/libavutil/pixfmt.h \
    Include/libavutil/random_seed.h \
    Include/libavutil/rational.h \
    Include/libavutil/rc4.h \
    Include/libavutil/replaygain.h \
    Include/libavutil/ripemd.h \
    Include/libavutil/samplefmt.h \
    Include/libavutil/sha.h \
    Include/libavutil/sha512.h \
    Include/libavutil/spherical.h \
    Include/libavutil/stereo3d.h \
    Include/libavutil/tea.h \
    Include/libavutil/threadmessage.h \
    Include/libavutil/time.h \
    Include/libavutil/timecode.h \
    Include/libavutil/timestamp.h \
    Include/libavutil/tree.h \
    Include/libavutil/twofish.h \
    Include/libavutil/version.h \
    Include/libavutil/xtea.h \
    Include/libpostproc/postprocess.h \
    Include/libpostproc/version.h \
    Include/libswresample/swresample.h \
    Include/libswresample/version.h \
    Include/libswscale/swscale.h \
    Include/libswscale/version.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -L$$PWD/Lib/ -lavcodec
win32: LIBS += -L$$PWD/Lib/ -lavdevice
win32: LIBS += -L$$PWD/Lib/ -lavfilter
win32: LIBS += -L$$PWD/Lib/ -lavformat
win32: LIBS += -L$$PWD/Lib/ -lavutil
win32: LIBS += -L$$PWD/Lib/ -lpostproc
win32: LIBS += -L$$PWD/Lib/ -lswresample
win32: LIBS += -L$$PWD/Lib/ -lswscale

INCLUDEPATH += $$PWD/Lib
DEPENDPATH += $$PWD/Lib
