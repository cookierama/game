#ifndef METEORITE_H
#define METEORITE_H
#include "Actor.h"
#include <QPolygon>


class CMeteorite : public CActor
{
public:
    CMeteorite();
	CMeteorite( const int x, const int y, const QVector2D dir );
	virtual void Draw( QPainter &painter, const float freq_tick );
	virtual bool checkCollision( const qreal x, const qreal y );
	QRect getBoundsT();
	const QPolygon getBodyT();

	static const int meteo_size = 8;
private:
	QPolygon m_body;
	void InitBody();
};

#endif // METEORITE_H
