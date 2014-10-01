#include "Meteorite.h"
#include "globals.h"

const QColor meteo_clr( 130, 110, 77 );


CMeteorite::CMeteorite()
{
	InitBody();
}
CMeteorite::CMeteorite( const int x, const int y, const QVector2D dir )
	: CActor( x, y, dir )
{
	InitBody();
}
void CMeteorite::InitBody()
{
	m_body.append( QPoint( 0, 0 ) );
	m_body.append( QPoint( mrand(  meteo_size/2, meteo_size ), -mrand( meteo_size/2, meteo_size ) ) );
	m_body.append( QPoint( meteo_size/2 - mrand( meteo_size/3, meteo_size/1.5 ), -mrand( meteo_size/2, meteo_size ) ) );
	m_body.append( QPoint( -mrand( meteo_size/2, meteo_size ), -mrand( 0, meteo_size/1.5 ) ) );
	m_body.append( QPoint( 0, 0 ) );
}

void CMeteorite::Draw( QPainter &painter, const float freq_tick )
{
	painter.setPen( QPen( meteo_clr, 1 ) );


	float tx = 0, ty = 0;
	getTickCoords( freq_tick, tx, ty );

	QPolygon m_tmp_bd( m_body );
	m_tmp_bd.translate( tx, ty );

	painter.drawPolygon( m_tmp_bd );

	QPainterPath pt;
	pt.addPolygon( m_tmp_bd );


	painter.fillPath( pt, QBrush( Qt::black ));
	painter.fillPath( pt, QBrush( meteo_clr, Qt::Dense2Pattern ));

}

bool CMeteorite::checkCollision( const qreal x, const qreal y )
{
	return m_body.containsPoint( QPoint( x, y ), Qt::OddEvenFill );
}

QRect CMeteorite::getBoundsT()
{
	QPolygon m_tmp_bd( m_body );
	m_tmp_bd.translate( getX(), getY() );
	return m_tmp_bd.boundingRect();
}

const QPolygon CMeteorite::getBodyT()
{
	QPolygon m_tmp_bd( m_body );
	m_tmp_bd.translate( getX(), getY() );
	return m_tmp_bd;
}
