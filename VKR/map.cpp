#include "map.h"
#include <QDebug>
#include <QList>
#include <QGraphicsItem>
#include "obstacle.h"
#include <QGraphicsScene>
#include <QTransform>
#include <QGraphicsView>
#include <QPainterPath>
#include <QScatterSeries>
#include <QElapsedTimer>
#include <QDebug>

Map::Map(QWidget *parent) : QGraphicsView (parent)
{
    this->setScene(new QGraphicsScene());
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setRenderHint(QPainter::Antialiasing);

    viewRenderTimer->setSingleShot(true);
    viewRenderTimer->start(50);
    connect(viewRenderTimer, SIGNAL(timeout()), this, SLOT(SceneInit()));

}

void Map::SceneInit()
{
    scene()->setSceneRect(this->rect());

    qDebug() << "Rect of wiev: " << this->rect();
    qDebug() << "Scene Rect: " << scene()->sceneRect();

    CreateFinishPoint(300, 300);
    CreateDiscreteMap();

    auv = new Vehicle();
    scene()->addItem(auv);

    auv->setPos(100, 100);
}

void Map::CreateFinishPoint(int x, int y)
{
    finishPoint = new QGraphicsEllipseItem(-10, -10, 20, 20);

    finishPoint->setFlag(QGraphicsItem::ItemIsMovable);

    finishPoint->setPen(QPen(Qt::darkRed));
    finishPoint->setBrush(Qt::red);

    scene()->addItem(finishPoint);

    finishPoint->setPos(x, y);
}

void Map::AddObstacle(Obstacle* obst, int x, int y, int rotation, double scale)
{
    scene()->addItem(obst);

    obst->setPos(x, y);
    obst->setRotation(rotation);
    obst->setScale(scale);

}

void Map::DeleteSelectedObstacles()
{
    QList<QGraphicsItem*> items = scene()->selectedItems();
    foreach (QGraphicsItem *item, items)
    {
        if (item->type() == Obstacle::Type)
            scene()->removeItem(item);
    }
}

void Map::DeleteAllObstacles()
{
    QList<QGraphicsItem*> items = scene()->items();
    foreach (QGraphicsItem *item, items)
    {
        if (item->type() == Obstacle::Type)
            scene()->removeItem(item);
    }
}

void Map::SetResolution(int resolution)
{
    if (resolution < 10)
        fillingStep = 3;
    else
        fillingStep = 2;

    this->resolution = resolution;

    FillDiscreteMap();
}

int Map::GetResolution()
{
    return resolution;
}

QVector<QVector<int> > Map::GetMapDots(QPoint &start, QPoint &finish)
{   
    const QGraphicsScene *map = scene();

    int countX = int(map->width() / resolution);
    int countY = int(map->height() / resolution);

    QVector<QVector<int>> mapPoints;
    mapPoints.resize(countX);

    for (auto iter = mapPoints.begin(); iter < mapPoints.end(); iter++)
        iter->resize(countY);

    QList<QGraphicsItem *>items;
    for (int x = 0; x < countX; x++)
        for (int y = 0; y < countY; y++)
        {
            items.clear();
            QRect rect(x * resolution  - resolution / 2, y * resolution - resolution/ 2, resolution, resolution);
            items = map->items(rect);

            if (items.size() == 0)
            {
                mapPoints[x][y] = DotType::passable;
                continue;
            }

            if (items[0]->type() == Obstacle::Type)
                mapPoints[x][y] = DotType::obstacle;
        }

    int finishX = int(finishPoint->x() / resolution);
    int finishY = int(finishPoint->y() / resolution);

    finish.setX(finishX);
    finish.setY(finishY);

    int startX = int(auv->pos().x() / resolution);
    int startY = int(auv->pos().y() / resolution);

    start.setX(startX);
    start.setY(startY);

    return mapPoints;
}

void Map::AddPath(QVector<QPoint> pathPoints, const QColor &color)
{
    if (pathPoints.size() == 0)
        return;

    QPainterPath path;
    path.moveTo(pathPoints[0].x() * resolution, pathPoints[0].y() * resolution);

    for(int i = 1; i < pathPoints.size(); i++)
    {
        path.lineTo(pathPoints[i].x() * resolution, pathPoints[i].y() * resolution);
    }

    QPen pen(QBrush(color), 3);

    QGraphicsItem *newPath = scene()->addPath(path, pen);
    newPath->setFlag(QGraphicsItem::ItemIsSelectable);
    allPaths << newPath;
}

void Map::DeleteSelectedPath()
{
    QList<QGraphicsItem*> items = scene()->selectedItems();
    foreach (QGraphicsItem *item, items)
    {
        if (item->type() != Obstacle::Type)
            scene()->removeItem(item);
    }
}

void Map::DeleteAllPaths()
{
    for (int i = 0; i < allPaths.size(); i++)
        scene()->removeItem(allPaths.at(i));
}

void Map::CreateDiscreteMap()
{
    discreteMap = new QChart();

    QRect rect(0, 0, this->width() / 3, this->height() / 3);
    discreteMap->setGeometry(rect);

    //discreteMap->setBackgroundPen(QPen(QColor(240, 240, 240)));
    discreteMap->setBackgroundBrush(QBrush(QColor(42, 42, 42)));

    this->scene()->addItem(discreteMap);

    discreteMap->setPos(this->width() - discreteMap->size().width(), this->height() - discreteMap->size().height());
    discreteMap->setMargins(QMargins(0, 0, 0, 0));
    discreteMap->legend()->hide();

    unknownPointsSeries = new QScatterSeries();
    unknownPointsSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    QPen pen;
    pen.setWidth(0);
    unknownPointsSeries->setPen(pen);
    unknownPointsSeries->setBrush(QBrush(Qt::gray));

    obstaclePointsSeries = new QScatterSeries();
    obstaclePointsSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    obstaclePointsSeries->setPen(pen);
    obstaclePointsSeries->setBrush(QBrush(Qt::darkRed));

    passablePointsSeries = new QScatterSeries();
    passablePointsSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    passablePointsSeries->setPen(pen);
    passablePointsSeries->setBrush(QBrush(Qt::darkGreen));

    AddSeries();

    FillDiscreteMap();
}

void Map::RemoveSeries()
{
    discreteMap->removeSeries(unknownPointsSeries);
    discreteMap->removeSeries(obstaclePointsSeries);
    discreteMap->removeSeries(passablePointsSeries);
}

void Map::AddSeries()
{
    discreteMap->addSeries(unknownPointsSeries);
    discreteMap->addSeries(obstaclePointsSeries);
    discreteMap->addSeries(passablePointsSeries);

    discreteMap->createDefaultAxes();

    discreteMap->axes(Qt::Horizontal).first()->setRange(0, this->width() / resolution);
    discreteMap->axes(Qt::Vertical).first()->setRange(0, this->height() / resolution);
    discreteMap->axes(Qt::Vertical).first()->setReverse(true);
}

void Map::FillDiscreteMap()
{
    qreal rangeX = this->width() / resolution;
    qreal rangeY = this->height() / resolution;

    unknownPointsSeries->setMarkerSize(resolution * fillingStep * 0.25);
    passablePointsSeries->setMarkerSize(resolution * fillingStep * 0.25);
    obstaclePointsSeries->setMarkerSize(resolution * fillingStep * 0.25);

    RemoveSeries();

    obstaclePointsSeries->clear();
    unknownPointsSeries->clear();
    passablePointsSeries->clear();

    for (qreal x(0); x < rangeX; x += fillingStep)
        for (qreal y(0); y < rangeY; y += fillingStep)
            *unknownPointsSeries << QPointF(x, y);

    AddSeries();
}

void Map::UpdateDiscreteMap(const QVector<QVector<int>> &mapPoints)
{
    QElapsedTimer timer;
    timer.start();

    RemoveSeries();

    QList<QPointF> unknownPoints = unknownPointsSeries->points();
    for (auto iter = unknownPoints.begin(); iter < unknownPoints.end(); iter++)
    {
        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::obstacle)
        {
            unknownPointsSeries->remove(*iter);
            obstaclePointsSeries->append(*iter);
        }

        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::passable)
        {
            unknownPointsSeries->remove(*iter);
            passablePointsSeries->append(*iter);
        }
    }

    QList<QPointF> obstaclePoints = obstaclePointsSeries->points();
    for (auto iter = obstaclePoints.begin(); iter < obstaclePoints.end(); iter++)
    {
        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::unknown)
        {
            obstaclePointsSeries->remove(*iter);
            unknownPointsSeries->append(*iter);
        }

        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::passable)
        {
            obstaclePointsSeries->remove(*iter);
            passablePointsSeries->append(*iter);
        }
    }

    QList<QPointF> passablePoints = passablePointsSeries->points();
    for (auto iter = passablePoints.begin(); iter < passablePoints.end(); iter++)
    {
        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::unknown)
        {
            passablePointsSeries->remove(*iter);
            unknownPointsSeries->append(*iter);
        }

        if (mapPoints[int(iter->x())][int(iter->y())] == DotType::obstacle)
        {
            passablePointsSeries->remove(*iter);
            obstaclePointsSeries->append(*iter);
        }
    }

    //  Выше все равно происходит перебор всех точек, поэтому
    //стоит опробовать метод, где просто очищаются все скаттеры,
    //а после идет их последовательное заполнение

    AddSeries();

    qDebug() << "Updating discrete map:" << timer.elapsed() << " ms" ;
}





















