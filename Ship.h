#ifndef SHIP_H
#define SHIP_H
#include "Actor.h"
#include <QPolygon>


class CShip : public CActor
{
public:
    CShip();
	virtual void Draw( QPainter &painter, const float freq_tick );
	virtual bool checkCollision( const qreal x, const qreal y );
	virtual bool checkCollision( const QPolygon mech );
	QRect getBoundsT();
	void stop();

private:
	QPolygon m_body;
};

#endif // SHIP_H
