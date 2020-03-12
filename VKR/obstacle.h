#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsItem>

class Obstacle : public QGraphicsItem
{
public:
    enum { Type = UserType + 1} ;
    int type() const override { return Type; }

    enum Form
    {
        circle,
        wall,
        square
    };
    Form form;

private:
    const int a = 200;
    const int b = 20;
    const int diameter = 100;

    QRect rect;

public:
    Obstacle(Form form);
    virtual ~Obstacle() override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

#endif // OBSTACLE_H
