#ifndef ACTOR_H
#define ACTOR_H
#include <QVector2D>
#include <QPainter>

class CActor
{
public:
    CActor();
	CActor( const int x, const int y, const QVector2D dir );

	// Position
	qreal getX();
	qreal getY();
	void setX( const qreal val );
	void setY( const qreal val );

	// Direction and speed
	void adjustDirection( const QVector2D dir );
	void adjustSpeed( const qreal multiplier );
	qreal getSpeed();

	void setMaxSpeed( const qreal speed );
	qreal getMaxSpeed();

	// Do a movement with direction and speed
	void stepDirection();

	// Get interpolated coords at FPS between FREQ ticks
	void getTickCoords( const float freq_bit, float& tx, float& ty );

	//Draw
	virtual void Draw( QPainter& painter, const float freq_tick ) = 0;
	virtual bool checkCollision( const qreal x, const qreal y ) = 0;

protected:
	qreal x;
	qreal y;
	QVector2D m_direction;
	qreal max_speed;
};

#endif // ACTOR_H
