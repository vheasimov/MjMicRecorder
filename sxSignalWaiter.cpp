#include <sxSignalWaiter.h>
#include <QCoreApplication>
#include <QTimerEvent>

class sxSignalWaiterPrivate : public sxPrivate<sxSignalWaiter>
{
public:
    sxSignalWaiterPrivate()
    {
        ready = false;
        emitted = false;
        timeout = false;
        waiting = false;
        timerID = 0;
    }

    bool ready, timeout, emitted, waiting;
    int timerID;

    void stopTimer()
    {
        if (timerID)
            qxt_p().killTimer(timerID);
        timerID = 0;
        waiting = false;
    }
};

//-------------------------------------------------------------------------
sxSignalWaiter::sxSignalWaiter(const QObject* sender, const char* signal) : QObject(0)
{
    Q_ASSERT(sender && signal);
    SX_INIT_PRIVATE(sxSignalWaiter);
    connect(sender, signal, this, SLOT(signalCaught()));
}
//-------------------------------------------------------------------------
bool sxSignalWaiter::wait(const QObject* sender, const char* signal, int msec, QEventLoop::ProcessEventsFlags flags)
{
    sxSignalWaiter w(sender, signal);
    return w.wait(msec, flags);
}
//-------------------------------------------------------------------------
bool sxSignalWaiter::wait(int msec, QEventLoop::ProcessEventsFlags flags)
{
    sxSignalWaiterPrivate& d = qxt_d();

    // Clear the emission status
    d.ready = false;
    d.emitted = false;

    // Check input parameters
    if (msec < -1 || msec == 0)
        return false;

    // activate the timeout
    if (msec != -1)
        d.timerID = startTimer(msec);
    else
        d.timerID = 0;

    // Make sure to wait for events
    flags |= QEventLoop::WaitForMoreEvents;

    // Begin waiting
    d.waiting = true;
    while (!d.ready && !d.timeout)
        QCoreApplication::processEvents(flags);

    // Clean up and return status
    qxt_d().stopTimer();
    d.emitted = d.ready;
    d.waiting = false;
    return d.ready;
}
//-------------------------------------------------------------------------
bool sxSignalWaiter::hasCapturedSignal() const
{
    return qxt_d().emitted;
}
//-------------------------------------------------------------------------
void sxSignalWaiter::signalCaught()
{
    if (!qxt_d().waiting) return;
    qxt_d().ready = true;
    qxt_d().stopTimer();
}
//-------------------------------------------------------------------------
void sxSignalWaiter::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    cancelWait();
}
//-------------------------------------------------------------------------
void sxSignalWaiter::cancelWait()
{
    if (!qxt_d().waiting) return;
    qxt_d().timeout = true;
    qxt_d().stopTimer();
}
//-------------------------------------------------------------------------