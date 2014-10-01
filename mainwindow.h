#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>

#include <QVector>
#include <QKeyEvent>

#include "Ship.h"
#include "Bullet.h"
#include "Meteorite.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	enum DIRECTION
	{
		STILL  = 0,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE
	};
signals:
	void signalBreak();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void SimulateConsistLogic( float ftime );
    void UpdateLogic( float ftime );

	void CheckGameRules( const float ftime = 0 );

    void paintEvent(QPaintEvent * pe )
    {
		paint_mx.lock();
        QMainWindow::paintEvent( pe );
		Render();
        paint_mx.unlock();

    }

	void UpdateBullet()
	{
		if ( m_allowbullet > 0 )
			m_allowbullet--;
		if ( m_allowmove > 0 )
			m_allowmove--;
	}

	void Render();

    QMutex paint_mx;
	QMutex close_mx;
private:
    Ui::MainWindow *ui;
	float freq, fps;
	//bool is_positive;
	int m_allowbullet;
	int m_allowmove;

	int score,
		fired,
		hits;

	QVector<DIRECTION> m_dir;

	QVector < CBullet > m_bullets;
	QVector< CMeteorite > m_enemies1;


	float freq_bit;
public slots:
	void DrawScene( float freq_bit )
    {
		this->freq_bit = freq_bit;
        repaint();
    }

    void setFREQ( float cnt )
    {
        freq = cnt;
    }

    void setFPS( float cnt )
    {
        fps = cnt;
    }

protected:
	//void keyPressEvent(QKeyEvent * event);


	void closeEvent( QCloseEvent *event )
	{
		emit signalBreak();
		close_mx.lock();
	}

	void keyReleaseEvent(QKeyEvent *keyEvent)
	{
		int i = 0;
		DIRECTION dr;

		switch( keyEvent->key() )
		{
		case Qt::Key_Up:
			dr = UP;
			break;
		case Qt::Key_Down:
			dr = DOWN;
			break;
		case Qt::Key_Left:
			dr = LEFT;
			break;
		case Qt::Key_Right:
			dr = RIGHT;
			break;
		case Qt::Key_Space:
			dr = SPACE;
			break;
		default:
			dr = STILL;
			break;
		}
		while( i < m_dir.size() )
		{
			if ( m_dir[ i ] == dr )
				break;
			i++;
		}

		if ( i != m_dir.size() )
		{
			m_dir.remove( i );
		}
	}//keyReleaseEvent

	void keyPressEvent(QKeyEvent *keyEvent)
	{
		qDebug( "keyPressEvent" );

		int i = 0;
		DIRECTION dr;

		switch( keyEvent->key() )
		{
		case Qt::Key_Up:
			dr = UP;
			break;
		case Qt::Key_Down:
			dr = DOWN;
			break;
		case Qt::Key_Left:
			dr = LEFT;
			break;
		case Qt::Key_Right:
			dr = RIGHT;
			break;
		case Qt::Key_Space:
			dr = SPACE;
			break;
		default:
			dr = STILL;
			break;
		}


		while( i < m_dir.size() )
		{
			if ( m_dir[ i ] == dr )
				break;
			i++;
		}

		if ( i == m_dir.size() )
		{
			m_dir.push_back( dr );

			QString strbtn;
			if ( dr == MainWindow::DOWN )
				strbtn = "DOWN";
			if ( dr == MainWindow::UP )
				strbtn = "UP";
			if ( dr == MainWindow::LEFT )
				strbtn = "LEFT";
			if ( dr == MainWindow::RIGHT )
				strbtn = "RIGHT";
			qDebug( QString("Pushed button " + strbtn + ". Index is %1").arg(m_dir.size() - 1 ).toAscii() );
		}


		//control_mx.unlock();

	}//keyPressEvent


public:
	CShip actor1;
};

#endif // MAINWINDOW_H
