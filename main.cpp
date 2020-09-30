#include "MjMicRecorder.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{

#ifndef _DEBUG
    char* argv2[20];
    for (int i = 0; i < argc; i++) {
        argv2[i] = argv[i];
    }
    int my_arg_count = 0;
    argv2[argc + my_arg_count++] = "-platformpluginpath";
    argv2[argc + my_arg_count++] = "./plugins";

    argv2[argc + my_arg_count] = nullptr;
    argc += my_arg_count;
    QApplication a(argc, argv2);
#else
    QApplication a(argc, argv);
#endif

#if QT_VERSION > 0x050000
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#else
    QApplication::setStyle(new QPlastiqueStyle);
#endif

    MjMicRecorder w;
    w.show();
    return a.exec();
}
