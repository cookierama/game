#include "mainthread.h"
#include <algorithm>
#include <QTime>
#include <cmath>

#include "Actor.h"

// Main worker thread file

void MainThread::run()
{

	qsrand( QDateTime::currentMSecsSinceEpoch() );
	is_break = false;
	w->close_mx.lock();

	//this->usleep( 1000 );

	// “актовый генератор: FREQ - логика, FPS - рендеринг
	const int FREQ = 1000 / 40; //  1000 / FPS
	const int MAX_FPS = 1000 / 180;

	qDebug( QString("Actual freq is approx %1").arg( (float)1000/FREQ ).toAscii() );
	qDebug( QString("Actual fps  is approx %1").arg( (float)1000/MAX_FPS ).toAscii() );

	const int MAX_LOOPS = 1000;
	const int AVG_INTERVAL = 3000; // интервал осреднени€ замеров производительности

	qint64 time_prev_tick = QDateTime::currentMSecsSinceEpoch();
	qint64 time_lastrender = time_prev_tick;

	// ћетки времени дл€ замера производительности
	qint64 time_freq = time_prev_tick;
	qint64 time_fps = time_prev_tick;

	int freq_cnt = 0, fps_cnt = 0;

	qint64 last_freq = time_prev_tick;

	while (true)
	{
		if ( is_break )
		{
			break;
		}
		qint64 time_cur_tick = QDateTime::currentMSecsSinceEpoch();

		int numLoops = 0;

		bool ft = true;
		while ( time_prev_tick < time_cur_tick && numLoops < MAX_LOOPS )
		{
			qDebug( QString("Logic called").toAscii() );			
			// ¬ызов логики
			w->UpdateLogic( 1 / FREQ );


			numLoops++;
			freq_cnt++;

			if ( ft )
			{
				ft = false;
				last_freq = time_cur_tick;
			}
			time_prev_tick += FREQ;

			// ќбновл€ем time_cur_tick дл€ более точного тактировани€
			time_cur_tick = QDateTime::currentMSecsSinceEpoch();

		}

		// здесь и далее дл€ ускорени€ не вызываем
		// currentMSecsSinceEpoch, а пользуемс€ полученными ранее
		// значени€ми
		qint64 time_tmp = QDateTime::currentMSecsSinceEpoch();
		if ( time_tmp - time_freq > AVG_INTERVAL )
		{
			float fqps = freq_cnt / ( (float)( time_tmp - time_freq ) / 1000 );
			freq_cnt = 0;
			time_freq = time_tmp;

			emit signalFREQ( fqps );
			qDebug( QString("%1").arg( fqps ).toAscii() );
		}

		time_tmp = QDateTime::currentMSecsSinceEpoch();
		w->SimulateConsistLogic( (float)( time_tmp - last_freq )/FREQ );

		time_tmp = QDateTime::currentMSecsSinceEpoch();
		if ( time_tmp - time_lastrender >= MAX_FPS &&
			 w->paint_mx.tryLock() )
		{

			time_lastrender = time_tmp;

			float freq_bit = 0;

			if (  time_tmp != last_freq )
				freq_bit = (float)( time_tmp - last_freq )/FREQ ;

			//qDebug( QString("NumLoops %1, Freq_bit = %2").arg(numLoops).arg(freq_bit).toAscii() );
			emit signalGUI( freq_bit );

			fps_cnt++;
			qint64 time_tmp = QDateTime::currentMSecsSinceEpoch();
			if ( time_tmp - time_fps > AVG_INTERVAL )
			{
				float fps = fps_cnt / ( (float)( time_tmp - time_fps ) / 1000 );
				fps_cnt = 0;
				time_fps = time_tmp;
				emit signalFPS( fps );
			}
			w->paint_mx.unlock();
		}
		// this->usleep( 300 );
	}

	w->close_mx.unlock();

}

