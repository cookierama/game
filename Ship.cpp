#include "Ship.h"
const QColor ship_clr( 98, 133, 156 );
const int ship_size = 4;

CShip::CShip()
{
	m_body.append( QPoint(          0, -ship_size ) );
	m_body.append( QPoint(  ship_size,  ship_size ) );
	m_body.append( QPoint( -ship_size,  ship_size ) );
	m_body.append( QPoint(          0, -ship_size ) );

}

void CShip::Draw( QPainter& painter, const float freq_tick )
{
	painter.setPen( QPen( Qt::blue, 2 ) );

	float tx = 0, ty = 0;
	getTickCoords( freq_tick, tx, ty );

	QPolygon m_tmp_bd( m_body );
	m_tmp_bd.translate( tx, ty );

	painter.drawPolygon( m_tmp_bd );

	painter.setPen( QPen( Qt::black, 3 ) );

	painter.drawLine( tx , ty - ship_size,
					  tx , ty - ship_size - 1);

	QPainterPath pt;
	pt.addPolygon( m_tmp_bd );

	painter.fillPath( pt, QBrush( ship_clr ));


}

bool CShip::checkCollision( const qreal x, const qreal y )
{
	return m_body.containsPoint( QPoint( x, y ), Qt::OddEvenFill );
}
bool CShip::checkCollision( const QPolygon mech )
{
	return ( mech.containsPoint( QPoint( getX(), getY() ), Qt::OddEvenFill ) ||
			 mech.containsPoint( QPoint( getX(), getY() + 1 ), Qt::OddEvenFill ) );
}


QRect CShip::getBoundsT()
{
	QPolygon m_tmp_bd( m_body );
	m_tmp_bd.translate( getX(), getY() );
	return m_tmp_bd.boundingRect();
}

void CShip::stop()
{
	m_direction.setX( 0 );
	m_direction.setY( 0 );
}
