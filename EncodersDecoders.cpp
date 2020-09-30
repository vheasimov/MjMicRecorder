#include <QAudioFormat>
#include <qendian.h>
#include "EncodersDecoders.h"
#include <qmath.h>


quint32 EncodersDecoders::PCM_To_MP3(lame_global_flags *gfp, unsigned char * mp3buf, const char * sour, quint32 nsamples)
{    
    quint32 sz = 0;
    int outBuf_size = 7200 + nsamples * 5 / 4;
    sz = lame_encode_buffer(gfp, (short int*)sour, (short int*)sour, nsamples, mp3buf, outBuf_size);
    return sz;                   
}

quint16 EncodersDecoders::calculationVolumeLevel(QAudioFormat format, char* data, quint32 nCounter)
{
    quint16 maxAmplitude = 0;
    if (format.sampleSize() == 16) {
        if (format.sampleType() == QAudioFormat::UnSignedInt)
            maxAmplitude = 0xFFFF;    
        else
            maxAmplitude = 0x7FFF;            
    } else if (format.sampleSize() == 8) {
        if (format.sampleType() == QAudioFormat::UnSignedInt)
            maxAmplitude = 0xFF;    
        else
            maxAmplitude = 0x7F;            
    } 

    if (maxAmplitude) {
        Q_ASSERT(format.sampleSize() % 8 == 0);
        const int channelBytes = format.sampleSize() / 8;
        const int sampleBytes = format.channelCount() * channelBytes;
        Q_ASSERT(nCounter % sampleBytes == 0);
        const int numSamples = nCounter / sampleBytes;

        quint16 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for(int j = 0; j < format.channelCount(); ++j) {
                quint16 value = 0;

                if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } 
                else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } 
                else if (format.sampleSize() == 16) {
                    if (format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, maxAmplitude);
        maxValue = 100*maxValue / maxAmplitude;
        return maxValue;
    }
    return 0;
}