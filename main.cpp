#include <QtGui/QApplication>
//#include "mainwindow.h"
#include "mainthread.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    MainThread th;
    th.w = &w;

	QObject::connect(&th, SIGNAL(signalGUI( float )), &w, SLOT(DrawScene( float )));
    QObject::connect(&th, SIGNAL(signalFREQ( float )), &w, SLOT(setFREQ( float )));
    QObject::connect(&th, SIGNAL(signalFPS( float )), &w, SLOT(setFPS( float )));

	QObject::connect(&w, SIGNAL(signalBreak()), &th, SLOT(setBreak()));


    th.start();

    return a.exec();
}
