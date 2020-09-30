#ifndef MjBuffer_h
#define MjBuffer_h

#include <QIODevice>

class MjBuffer : public QIODevice
{
    Q_OBJECT

public:
    MjBuffer(quint64 size = 1048576, QObject *parent = NULL);
    ~MjBuffer();
    void start();
    void stop();
    qint64 readData(char * data, qint64 len);
    qint64 read(char * data, qint64 len);
    qint64 readBlock (char * data, quint64 size);
    qint64 writeData(const char *data, qint64 len);
    qint64 write(const char *data, qint64 len);
    qint64 bytesAvailable() const;
private:
    const char * m_buffer;
    qint64 m_readCursor;
    qint64 m_writeCursor;
    qint64 m_bufSize;
    qint64 m_bytesReady;

};
#endif // MjBuffer_h
