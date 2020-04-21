#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

#include <QGraphicsItem>
#include <QPolygon>
#include <QGraphicsScene>
#include <AUV/su_rov.h>

class Vehicle : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    SU_ROV *ROV;

private:
    struct Point
    {
        double x;
        double y;
    };

    Point coordinates;
public:
    void SetCoordinates(double x, double y);


private:
    double courseAngle = 0;

public:
    double sonar_range = 200;                     // Дистанция сканирования для ГСО
    double sonar_viewAngle = 120;                // Угол обзора ГСО
    static const int sonar_numberOfBeams = 120;  // Количество лучей, испускаемых ГСО
    static constexpr double sonar_rangeStep = 1;

    double sonar_data[sonar_numberOfBeams];
public:
    void UpdateSonarData();

public:
    void SetCourseAngle(double angle);
    void MoveForward(double speed);

    void MoveToPoint(double x, double y);

private slots:
    void UpdateDataFromROV();

private:
    QGraphicsScene *map = nullptr;
public:
    explicit Vehicle();
    virtual ~Vehicle();

private:
    QPolygon form;
public:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

#endif // VEHICLE_H
