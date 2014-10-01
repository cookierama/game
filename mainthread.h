#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include "mainwindow.h"


class MainThread : public QThread
{
    Q_OBJECT
public:
    MainWindow* w;
protected:
    //MyThread( MainWindow* mw );
    virtual void run();

	bool is_break;

signals:
	void signalGUI( float freq_bit );
    void signalFPS( float cnt );
    void signalFREQ( float cnt );
public slots:
	void setBreak()
	{
		is_break = true;
	}
};

#endif // MAINTHREAD_H
