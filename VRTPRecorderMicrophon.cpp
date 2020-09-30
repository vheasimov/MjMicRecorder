#include "VRTPRecorderMicrophon.h"

#include <QFile>
#include <QtMultimedia/QAudioDeviceInfo>
#include "sxSignalWaiter.h"


enum class VBufSize{
    mono8000 = 220,
    mono11025 = 442,
    mono16000 = 642,
    mono22050 = 882,
    mono44100 = 2*mono22050,
    stereo44100 = 2*mono44100,
};

VRTPRecorderMicrophon::VRTPRecorderMicrophon(QObject *parent, const QString & destFileName)
{
    Q_UNUSED(parent);
    m_filename = destFileName;
}

VRTPRecorderMicrophon::~VRTPRecorderMicrophon()
{
}

void VRTPRecorderMicrophon::run()
{
    initMP3();
    m_stop = false;

    m_audioInput = std::make_shared<QAudioInput>(m_audioFormat);
    m_destinationFile = std::make_shared<QFile>(m_filename);
    if (m_destinationFile->open(QIODevice::WriteOnly) == false) {
        qDebug() << QString("Can't open file %1 from write").arg(m_filename);
        return;
    }

    quint64 nCounter = 0;
    std::vector<char> data(10000, 0);
    auto m_audioBuffer = std::make_shared<MjBuffer>();
    m_audioBuffer->start();
    m_audioInput->start(m_audioBuffer.get());

    qDebug() << QString("Start Voice Recording");    
    sxSignalWaiter::wait(this, SIGNAL(nullsignal()), 100);
    while(m_stop == false){
        nCounter = 0;
        if (m_audioBuffer == NULL)
            break;

        if (m_audioBuffer->bytesAvailable() >= static_cast<int>(VBufSize::mono44100)) {
            nCounter = m_audioBuffer->read(data.data(), static_cast<int>(VBufSize::mono44100));
		}
        if (nCounter > 0) {
            quint8 level = EncodersDecoders::calculationVolumeLevel(m_audioFormat, data.data(), nCounter);
            emit changeLevel(level);
            convertAndSaveData(data.data(), nCounter);
        }
        sxSignalWaiter::wait(this, SIGNAL(nullsignal()), 10);
    }
    destroyStreamer();
    qDebug() << QString("Stop Rcording Voice");
}

quint32 VRTPRecorderMicrophon::convertAndSaveData(char *data, quint32 nCounter)
{
    std::vector<char> buffer(static_cast<int>(VBufSize::mono44100), 0);
    nCounter = EncodersDecoders::PCM_To_MP3(gfp, (unsigned char*)(buffer.data()), data, nCounter / 2);
    if (nCounter <= 0) {
        return 0;
	}

    m_destinationFile->write(buffer.data(), nCounter);
	static uint64_t counter {0};
    if (++counter % 10 == 0) {
        m_destinationFile->flush();
    }
    return nCounter;
}

void VRTPRecorderMicrophon::initMP3()
{
    gfp = lame_init();
    lame_set_num_channels(gfp,1);
    lame_set_out_samplerate(gfp, 44100);
    lame_set_in_samplerate(gfp, 44100);
    lame_set_brate(gfp,128);
    lame_set_mode(gfp,STEREO);
    lame_set_quality(gfp,2);
    lame_set_bWriteVbrTag(gfp, 0);
    lame_init_params(gfp);
    
    m_audioFormat.setSampleRate(44100);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);
}

void VRTPRecorderMicrophon::destroyStreamer()
{
    lame_close(gfp); 
    if (m_destinationFile) {
        m_destinationFile->close();
        m_destinationFile.reset();
    }

    if (m_audioInput) {
        m_audioInput->stop();
        m_audioInput->deleteLater();
        m_audioInput.reset();
    }
}

void VRTPRecorderMicrophon::stop() 
{
    m_mutex.lock();
    m_stop = true;
    m_mutex.unlock();
}