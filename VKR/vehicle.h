#ifndef VEHICLE_H
#define VEHICLE_H

#include <QGraphicsItem>
#include <QPolygon>

class Vehicle : public QGraphicsItem
{
private:
    QPolygon form;

public:
    Vehicle();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

#endif // VEHICLE_H
