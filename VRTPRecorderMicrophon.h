#ifndef VRTPRecorderMicrophon_h
#define VRTPRecorderMicrophon_h

#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioFormat>
#include <QFile>
#include <QThread>
#include <QMutex>

#include "MjBuffer.h"
#include "EncodersDecoders.h"

class VRTPRecorderMicrophon : public QThread
{
    Q_OBJECT

public:

    VRTPRecorderMicrophon(QObject *parent, const QString & destFileName);
    virtual ~VRTPRecorderMicrophon();

    void run();
    void stop();

signals:
    void nullsignal();
    void stopTransmit();
    void changeLevel(quint8 level);

private:
    void destroyStreamer();
    
    void initMP3();
    
    quint32 convertAndSaveData(char *data, quint32 nCounter);

    
    std::shared_ptr<QAudioInput> m_audioInput;
    QAudioFormat m_audioFormat;
    
    lame_global_flags *gfp;
    std::shared_ptr<QFile> m_destinationFile;

    bool m_stop;

QMutex m_mutex;

QString m_filename;
};
#endif // VRTPSenderMicrophon_h
