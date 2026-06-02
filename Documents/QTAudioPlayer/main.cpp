#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

