#ifndef BULLET_H
#define BULLET_H
#include "Actor.h"
#include <QPolygon>


class CBullet : public CActor
{
public:
    CBullet();
	CBullet( const int x, const int y, const QVector2D dir );
	virtual void Draw( QPainter &painter, const float freq_tick );
	virtual bool checkCollision( const qreal x, const qreal y );
	virtual bool checkCollision( const QPolygon mech );
};

#endif // BULLET_H
