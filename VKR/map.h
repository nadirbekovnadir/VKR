#ifndef MAP_H
#define MAP_H

#include <QGraphicsView>
#include <QTimer>
#include "obstacle.h"
#include <QVector>
#include <QGraphicsEllipseItem>
#include "vehicle.h"
#include <QColor>
#include <QtCharts>
#include <QChart>

class Map : public QGraphicsView
{
    Q_OBJECT

public:
    Map(QWidget *parent = nullptr);

private:
    QTimer *viewRenderTimer = new QTimer();

private slots:
    void SceneInit();

private:
    QGraphicsEllipseItem *finishPoint;
    void CreateFinishPoint(int x, int y);

    Vehicle *auv;

public:
    void AddObstacle(Obstacle* obst, int x, int y, int rotation, double scale);
    void DeleteSelectedObstacles();
    void DeleteAllObstacles();

private:
    int resolution = 10;
public:
    void SetResolution(int resolution);
    int GetResolution();

public:
    enum DotType
    {
        unknown = -2,
        obstacle = -1,
        passable = 0,
    };

    QVector<QVector<int>> GetMapDots(QPoint *start, QPoint *finish);

    QVector<QGraphicsItem*> allPaths;
    void AddPath(QVector<QPoint> path, const QColor &color);
    void DeleteSelectedPath();
    void DeleteAllPaths();

private:
    QChart *discreteMap;
    QScatterSeries *unknownPointsSeries;
    QScatterSeries *obstaclePointsSeries;
    QScatterSeries *passablePointsSeries;
    void CreateDiscreteMap();

    void RemoveSeries();
    void AddSeries();

    int fillingStep = 2;
    void FillDiscreteMap();
public:
    void UpdateDiscreteMap(const QVector<QVector<int>> &mapPoints);

};

#endif // MAP_H
