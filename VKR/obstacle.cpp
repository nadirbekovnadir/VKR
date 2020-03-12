#include "obstacle.h"
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

Obstacle::Obstacle(Form form)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);

    this->form = form;

    if (form == wall)
        rect.setRect(-a / 2, -b / 2, a, b);
    else if (form == circle || form == square)
        rect.setRect(-diameter / 2, -diameter / 2, diameter, diameter);
}

Obstacle::~Obstacle()
{

}

QRectF Obstacle::boundingRect() const
{
    QRect bRect(rect.topLeft() - QPoint(1, 1), rect.bottomRight() + QPoint(1, 1));
    return bRect;
}

QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    if (form == wall || form == square)
        path.addRect(rect);
    else if (form == circle)
        path.addEllipse(rect);

    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen pen;
    pen.setWidth(3);

    if ((option->state & QStyle::State_Selected) == 0)
       pen.setColor(Qt::darkGray);
    else
    {
        pen.setColor(Qt::white);
        pen.setStyle(Qt::DotLine);
    }

    painter->setPen(pen);
    painter->setBrush(Qt::gray);

    if (form == wall || form == square)
        painter->drawRect(rect);
    else if (form == circle)
        painter->drawEllipse(rect);
}

