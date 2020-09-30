#include "MjBuffer.h"


MjBuffer::MjBuffer(quint64 size, QObject *parent) 
: QIODevice(parent) 
{
    m_bufSize = size;
    m_buffer = new char[m_bufSize];
    m_readCursor   = 0;
    m_writeCursor  = 0;
    m_bytesReady = 0;
}

MjBuffer::~MjBuffer() 
{
    delete[] m_buffer;
    m_buffer = NULL;
}

void MjBuffer::start() 
{
    if(!isOpen())
        open(QIODevice::ReadWrite);
}

void MjBuffer::stop() 
{
    close();
}

qint64 MjBuffer::readData(char * data, qint64 len) 
{
    if (len > m_bufSize)
        len = m_bufSize;

    if(m_bytesReady <= 0)
        return 0;

    if(len > m_bytesReady)
        len = m_bytesReady;
    int rightBytes = (m_bufSize - m_readCursor);

    if(len < rightBytes) {
        memcpy(data,(void*)&m_buffer[m_readCursor],len);
        m_readCursor += len;
        m_bytesReady -= len;

        return len;
    }
    else {
        memcpy(data,(void*)&m_buffer[m_readCursor],rightBytes);
        int leftBytes = len-rightBytes;
        m_readCursor = 0;
        m_bytesReady -= len;

        memcpy(data,(void*)&m_buffer[m_readCursor],leftBytes);
        m_readCursor += leftBytes;
        m_bytesReady -= leftBytes;
    }
    return len;
}

qint64 MjBuffer::read(char * data, qint64 len)
{
    return readData(data,len);
}

qint64 MjBuffer::readBlock (char * data, quint64 size)
{
    return readData(data,size);
}

qint64 MjBuffer::writeData(const char *data, qint64 len)
{
    if (len <= 0) {
        return 0;
    }
   
    if (len > m_bufSize) {
        len = m_bufSize;
    }

    int freeBytes = (m_bufSize - m_writeCursor);

    if(len < freeBytes) {
        memcpy((void*)&m_buffer[m_writeCursor],data,len);
        m_writeCursor += len;
        m_bytesReady += len;
        emit readyRead();
        return len;
    }
    else {
        memcpy((void*)&m_buffer[m_writeCursor],data,freeBytes);
        m_bytesReady += freeBytes;
        m_writeCursor = 0;

        int leftBytes = len-freeBytes;
        memcpy((void*)&m_buffer[m_writeCursor],data,leftBytes);
        m_writeCursor += leftBytes;
        m_bytesReady += leftBytes;

        emit readyRead();
        return len;
    }
}

qint64 MjBuffer::write(const char *data, qint64 len)
{
    return writeData(data,len);
}

qint64 MjBuffer::bytesAvailable() const
{
    return m_bytesReady;
}