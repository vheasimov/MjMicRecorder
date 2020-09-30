#ifndef MjSignalWaiter_H
#define MjSignalWaiter_H

#include <QtGlobal>
#include <QObject>
#include <QEventLoop>


#define SX_VERSION 0x000000
#define SX_VERSION_STR "0.0.1"

#define SX_DECLARE_PRIVATE(PUB) friend class PUB##Private; sxPrivateInterface<PUB, PUB##Private> qxt_d;
#define SX_DECLARE_PUBLIC(PUB) friend class PUB;
#define SX_INIT_PRIVATE(PUB) qxt_d.setPublic(this);
#define SX_D(PUB) PUB##Private& d = qxt_d()
#define SX_P(PUB) PUB& p = qxt_p()

template <typename PUB>
class sxPrivate
{
public:
    virtual ~sxPrivate()
    {}
    inline void QXT_setPublic(PUB* pub)
    {
        qxt_p_ptr = pub;
    }

protected:
    inline PUB& qxt_p()
    {
        return *qxt_p_ptr;
    }
    inline const PUB& qxt_p() const
    {
        return *qxt_p_ptr;
    }

private:
    PUB* qxt_p_ptr;
};


template <typename PUB, typename PVT>
class sxPrivateInterface
{
    friend class sxPrivate<PUB>;
public:
    sxPrivateInterface()
    {
        pvt = new PVT;
    }
    ~sxPrivateInterface()
    {
        delete pvt;
    }

    inline void setPublic(PUB* pub)
    {
        pvt->QXT_setPublic(pub);
    }
    inline PVT& operator()()
    {
        return *static_cast<PVT*>(pvt);
    }
    inline const PVT& operator()() const
    {
        return *static_cast<PVT*>(pvt);
    }
private:
    sxPrivateInterface(const sxPrivateInterface&) { }
    sxPrivateInterface& operator=(const sxPrivateInterface&) { }
    sxPrivate<PUB>* pvt;
};


class MjSignalWaiterPrivate;

class MjSignalWaiter : public QObject
{
    Q_OBJECT
    SX_DECLARE_PRIVATE(MjSignalWaiter)
public:
    MjSignalWaiter(const QObject* sender, const char* signal);

    static bool wait(const QObject* sender, const char* signal, int msec = -1, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);


    bool wait(int msec = -1, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);
    bool hasCapturedSignal() const;

public Q_SLOTS:
    void signalCaught();
    void cancelWait();

Q_SIGNALS:
    void nullSignal();

private:
    void timerEvent(QTimerEvent* event);
};

#endif
