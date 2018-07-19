#include "healthsystem.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <mainwindows.h>
int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    QFile styleSheet("://qss/health.qss");
    if(!styleSheet.open(QFile::ReadOnly)){
        qWarning("can not open style file");
    }
    a.setStyleSheet(styleSheet.readAll());
    styleSheet.close();

    MainWindows m;
    m.setMinimumSize(860,580);
    m.show();
    return a.exec();
}
