#include "AUV/vehicle.h"
#include <QPoint>
#include <QRect>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include "map.h"
#include <QList>
#include <QGraphicsItem>
#include "obstacle.h"

void Vehicle::SetCoordinates(double x, double y)
{
    this->setPos(x, y);
    coordinates.x = x;
    coordinates.y = y;
}

void Vehicle::UpdateSonarData()
{
    if (map == nullptr)
        map = this->scene();

    QList<QGraphicsItem *>items;

    double tempCos;
    double tempSin;
    double searchAngle;

    double tempX;
    double tempY;
    bool obstacleDetected = false;

    searchAngle = courseAngle - sonar_viewAngle/2;

    for (int i = 0; i < sonar_numberOfBeams; i++)
    {
        obstacleDetected = false;

        sonar_data[i] = sonar_range;
        searchAngle += sonar_viewAngle / sonar_numberOfBeams;
        tempCos = qCos(searchAngle / 180 * M_PI);
        tempSin = qSin(searchAngle / 180 * M_PI);

        tempX = coordinates.x;
        tempY = coordinates.y;

        for (double length = 0; length <= sonar_range; length += sonar_rangeStep)
        {
            tempX += sonar_rangeStep * tempCos;
            tempY += sonar_rangeStep * tempSin;

            items = map->items(QPointF(tempX, tempY));

            for (auto iter = items.begin(); iter != items.end(); iter++)
            {
                if ((*iter)->type() == Obstacle::Type)
                {
                    obstacleDetected = true;
                    break;
                }

            }

            if (obstacleDetected)
            {
                sonar_data[i] = length;
                break;
            }
        }
    }
}

void Vehicle::UpdateCourseAngle(double angle)
{
    //ROV->setKurs(angle);
}

Vehicle::Vehicle()
{
    setFlag(ItemIsMovable);

    form << QPoint(15, 0)
         << QPoint(-15, -10)
         << QPoint(-15, 10);

    ROV = new SU_ROV();
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
