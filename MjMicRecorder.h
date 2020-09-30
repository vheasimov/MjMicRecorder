#ifndef MJMICRECORDER_H
#define MJMICRECORDER_H

#include <QtWidgets/QMainWindow>

#include "VRTPRecorderMicrophon.h"

#include "ui_MjMicRecorder.h"

class MjMicRecorder : public QMainWindow
{
    Q_OBJECT

public:
    MjMicRecorder(QWidget *parent = nullptr);
    ~MjMicRecorder();

public slots:
    void startRecording();
    void stopRecording();
    void onTimer();
private:
    Ui::MjMicRecorderClass ui;

    VRTPRecorderMicrophon * recorder_{ nullptr };
};

#endif // MJMICRECORDER_H
