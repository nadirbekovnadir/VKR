#include "vehicle.h"
#include <QPoint>
#include <QRect>
#include <QPainterPath>
#include <QPainter>
#include <QPen>

Vehicle::Vehicle()
{
    setFlag(ItemIsMovable);

    form << QPoint(15, 0)
         << QPoint(-15, - 10)
         << QPoint(-15, 10);

}

QRectF Vehicle::boundingRect() const
{
    QRect rect(-15, -10, 30, 20);
    return rect;
}

QPainterPath Vehicle::shape() const
{
    QPainterPath path;
    path.addPolygon(form);
    return path;
}

void Vehicle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    painter->setPen(QPen(Qt::darkGreen));
    painter->setBrush(Qt::green);
    painter->drawPolygon(form);
}
