#ifndef EncodersDecoders_h
#define EncodersDecoders_h

#include <QtGlobal>
#include <QDebug>

#include <lame/lame.h>

class QAudioFormat;

class EncodersDecoders{
public:  
    static quint16 calculationVolumeLevel(QAudioFormat format, char* data, quint32 nCounter);
    static quint32 PCM_To_MP3(lame_global_flags *gfp, unsigned char * mp3buf, const char * sour, quint32 nsamples);
};


#endif // EncodersDecoders_h
