#include "MjMicRecorder.h"
#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include "sxSignalWaiter.h"


MjMicRecorder::MjMicRecorder(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.start, SIGNAL(clicked()), this, SLOT(startRecording()));
    connect(ui.stop, SIGNAL(clicked()), this, SLOT(stopRecording()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(1000);
}

MjMicRecorder::~MjMicRecorder()
{
    stopRecording();
}


void MjMicRecorder::startRecording()
{
    if (recorder_) {
        return;
    }
    QString filename = QCoreApplication::applicationDirPath() + QString("/sounds/record_%1.mp3").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
    recorder_ = new VRTPRecorderMicrophon(this, filename);
    recorder_->start();
}

void MjMicRecorder::stopRecording()
{
    if (recorder_) {
        recorder_->stop();
        sxSignalWaiter::wait(recorder_, SIGNAL(nullsignal()), 100);
        recorder_->deleteLater();
        recorder_ = nullptr;
    }
}

void MjMicRecorder::onTimer()
{
    QTime tmm = QTime::currentTime();
    if (tmm.minute() == 0 && tmm.second() == 0)
    {
        if (recorder_)
        {
            stopRecording();
            startRecording();
        }
    }
}
