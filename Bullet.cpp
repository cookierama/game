#include "Bullet.h"

CBullet::CBullet()
{
}
CBullet::CBullet( const int x, const int y, const QVector2D dir )
	: CActor( x, y, dir )
{
}

void CBullet::Draw( QPainter &painter, const float freq_tick )
{

	float tx = 0, ty = 0;
	getTickCoords( freq_tick, tx, ty );





	painter.setPen( QPen( Qt::black, 2 ) );

	painter.drawLine( tx, ty,
					  tx, ty + 1 );
}

bool CBullet::checkCollision( const qreal x, const qreal y )
{
	return  ( x - getX() < 0.001 && ( y - getY() < 0.001 || y - getY() + 1 < 0.001 ) ) ;
}

bool CBullet::checkCollision( const QPolygon mech )
{
	return ( mech.containsPoint( QPoint( getX(), getY() ), Qt::OddEvenFill ) ||
			 mech.containsPoint( QPoint( getX(), getY() + 1 ), Qt::OddEvenFill ) );
}
