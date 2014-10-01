#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"

#include "QTime"

#include "cmath"
#include "globals.h"
const int max_enemies = 6;

const float MAX_INERTIA = 10;
float inertia = MAX_INERTIA;

const int field_w = 250;
const int field_h = 480;
const int field_ident = 5;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setFocus();
	m_allowbullet = true;
	m_allowmove = 0;


	actor1.setMaxSpeed( 7 );
	actor1.setX( ( field_w - field_ident ) / 2 );
	actor1.setY( field_h - field_ident );

	score = fired = hits = freq = fps = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::UpdateLogic( float ftime )
{

	const float speed_const = 2;

	float speed = speed_const; //можно добавить инерцию при разгоне

	for ( int i = 0; i < m_dir.size(); i++ )
	{

		if ( m_dir[i] == MainWindow::UP )
			actor1.adjustDirection( QVector2D( 0, -speed ) );
		if ( m_dir[i] == MainWindow::DOWN )
			actor1.adjustDirection( QVector2D( 0, speed ) );
		if ( m_dir[i] == MainWindow::LEFT )
			actor1.adjustDirection( QVector2D( -speed, 0 ) );
		if ( m_dir[i] == MainWindow::RIGHT )
			actor1.adjustDirection( QVector2D( speed, 0 ) );

		m_allowmove=3;

		if ( m_dir[i] == MainWindow::SPACE &&
			 m_allowbullet == 0 )
		{
			m_bullets.push_back( CBullet( actor1.getX(), actor1.getY() - 1, QVector2D( 0, -15) ) );
			qDebug( QString("Added bullet. Pos %1").arg( m_bullets.size() - 1 ).toAscii() );
			m_allowbullet = 5;
			fired++;
		}


	}

	actor1.stepDirection();



	bool dir_touched = false;
	for ( int i = 0; i < m_dir.size(); i++ )
	{
		if (m_dir[i] != MainWindow::SPACE )
		{
			dir_touched = true;
			break;
		}
	}

	if ( !dir_touched )
	{
		float inertia = 0.5;
		if ( actor1.getSpeed() < 0.5 )
			inertia = 1;

		actor1.adjustSpeed( inertia);
	}

	for ( int i = 0; i < m_bullets.size(); i++ )
	{
		m_bullets[ i ].stepDirection();
	}

	for ( int x = 0; x < m_enemies1.size(); x++ )
	{
		m_enemies1[ x ].stepDirection();
	}

	CheckGameRules();
	if ( m_enemies1.size() < max_enemies )
	{
		CMeteorite meteo( mrand( field_ident + CMeteorite::meteo_size,
								 field_ident + field_w - CMeteorite::meteo_size ),
						  -mrand( 0, 20 ),
					  QVector2D( 0, 1 ) );
		while( true )
		{
			int i = 0;
			while( i < m_enemies1.size() )
			{
				if( meteo.getBoundsT().intersects( m_enemies1[ i ].getBoundsT() ) )
					break;

				i++;
			}

			if ( i == m_enemies1.size() )
				break;


			meteo = CMeteorite( mrand( 1, 100 ), -mrand( 0, 20 ),
								  QVector2D( 0, 1 ) );

		}
		m_enemies1.push_back( meteo );
	}


	UpdateBullet();


}


void MainWindow::CheckGameRules( const float ftime )
{
	QRect field_rect( field_ident, field_ident,
					  field_w,
					  field_h );

	for ( int i = 0; i < m_bullets.size(); i++ )
	{
		CBullet blt = m_bullets[ i ];
		float tx = 0, ty = 0;
		blt.getTickCoords( ftime, tx, ty );

		blt.setX( tx );
		blt.setY( ty );

		if ( !field_rect.contains( m_bullets[ i ].getX(), m_bullets[ i ].getY() ) )
		{
			m_bullets.remove( i-- );
		}
		else
		{
			for ( int j = 0; j < m_enemies1.size(); j++ )
			{
				CMeteorite enm = m_enemies1[ j ];
				float etx = 0, ety = 0;
				enm.getTickCoords( ftime, etx, ety );
				enm.setX( etx );
				enm.setY( ety );

				if ( blt.checkCollision( enm.getBodyT() ) )
				{
					m_enemies1.remove( j-- );
					m_bullets.remove( i-- );
					score++;
					break;
				}
			} //for
		}
	}

	for ( int j = 0; j < m_enemies1.size(); j++ )
	{

		CMeteorite enm = m_enemies1[ j ];
		if ( !field_rect.contains( enm.getBoundsT() ) &&
			 field_rect.bottomRight().y() < enm.getBoundsT().topLeft().y() )
		{
			m_enemies1.remove( j-- );
		}

		if ( actor1.checkCollision( enm.getBodyT() ) )
		{
			m_enemies1.remove( j-- );
			hits++;
		}
	}


	if ( !field_rect.contains( actor1.getBoundsT(), true ) )
	{
		while ( field_rect.x() >= actor1.getBoundsT().left() )
			actor1.setX( actor1.getX() + 1 );

		while ( field_rect.x()*2 + field_rect.width() <= actor1.getBoundsT().x() + actor1.getBoundsT().width())
			actor1.setX( actor1.getX() - 1 );

		while ( field_rect.top() >= actor1.getBoundsT().top() )
			actor1.setY( actor1.getY() + 1 );

		while ( field_rect.y()*2 + field_rect.height() <= actor1.getBoundsT().y() + actor1.getBoundsT().height())
			actor1.setY( actor1.getY() - 1 );

		actor1.stop();

	}


}

void MainWindow::SimulateConsistLogic( float ftime )
{
	for ( float bt = 0; bt < ftime; bt = bt + 0.01 )
	{
		CheckGameRules( bt );
	}
}

void MainWindow::Render()
{

    QPainter qpainter(this);

	const int bgw = 2;
	qpainter.setPen (QPen(Qt::black, bgw));
	qpainter.setBrush( QBrush( Qt::darkGray ) );
	qpainter.drawRect( field_ident, field_ident,
					   field_w + field_ident,
					   field_h + field_ident );

    qpainter.setPen (QPen(Qt::blue, 3));

	qpainter.setPen (QPen(Qt::black, 3));
	for ( int i = 0; i < m_bullets.size(); i++ )
	{
		CBullet blt = m_bullets[ i ];
		blt.Draw( qpainter, freq_bit );
	}

	qpainter.setPen (QPen(Qt::red, 1));

	for ( int i = 0; i < m_enemies1.size(); i++ )
	{
		CMeteorite enm = m_enemies1[ i ];		
		enm.Draw( qpainter, freq_bit );
	}

	// draw player	
	actor1.Draw( qpainter, freq_bit );


	QPalette pal;
	qpainter.setBrush( pal.brush( QPalette::Window ) );
	qpainter.setPen (QPen(pal.color( QPalette::Window), 1));
	qpainter.drawRect( field_ident - bgw/2, 0,
					   field_w + field_ident + bgw/2,
					   field_ident - bgw );

	qpainter.setPen (QPen(Qt::black, bgw));
	qpainter.setBrush( QBrush( Qt::darkGray, Qt::NoBrush ));
	qpainter.drawRect( field_ident, field_ident,
					   field_w + field_ident,
					   field_h + field_ident );


	ui->label_freq->setText( QString("%1").arg( freq ).toAscii() );
	ui->label_fps->setText( QString("%1").arg( fps ).toAscii() );
	ui->label_speed->setText( QString("%1").arg( actor1.getSpeed() ).toAscii() );

	ui->label_score->setText( QString("%1").arg( score ).toAscii() );
	ui->label_fired->setText( QString("%1").arg( fired ).toAscii() );
	ui->label_hits->setText( QString("%1").arg( hits ).toAscii() );


}


