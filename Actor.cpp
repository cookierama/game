#include "Actor.h"
#include "math.h"

CActor::CActor()
{
	x = 0;
	y = 0;
	m_direction.setX( 0 );
	m_direction.setY( 0 );
	max_speed = 0;
}
CActor::CActor( const int x, const int y, const QVector2D dir )
{
	this->x = x;
	this->y = y;
	this->m_direction = dir;
}

qreal CActor::getX()
{
	return x;
}
qreal CActor::getY()
{
	return y;
}
void CActor::setX( const qreal val )
{
	x = val;
}
void CActor::setY( const qreal val )
{
	y = val;
}
void CActor::adjustSpeed( const qreal multiplier )
{
	adjustDirection( QVector2D( -m_direction.x(), -m_direction.y() ) * multiplier );
}

void CActor::adjustDirection( const QVector2D dir )
{
	m_direction += dir;

	if ( m_direction.length() > max_speed )
	{
		float mlt = ( m_direction.length() - max_speed ) / m_direction.length();
		m_direction.setX( m_direction.x() - m_direction.x() * mlt );
		m_direction.setY( m_direction.y() - m_direction.y() * mlt );
	}
}
void CActor::stepDirection()
{
	x += m_direction.x();
	y += m_direction.y();
}

void CActor::getTickCoords( const float freq_bit, float& tx, float& ty )
{
	QVector2D tmp = m_direction;
	qreal x = getX(), y = getY();
	m_direction.setX( m_direction.x() * freq_bit );
	m_direction.setY( m_direction.y() * freq_bit );
	stepDirection();
	tx = getX();
	ty = getY();

	setX( x );
	setY( y );
	m_direction = tmp;
}

qreal CActor::getSpeed()
{
	return m_direction.length();
}

void CActor::setMaxSpeed( const qreal speed )
{
	max_speed = speed;
}

qreal CActor::getMaxSpeed()
{
	return max_speed;
}
