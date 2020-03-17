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
    this->resolution = resolution;
}

int Map::GetResolution()
{
    return resolution;
}

QVector<QVector<int> > Map::GetMapDots(QPoint *start, QPoint *finish)
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

    finish = new QPoint(finishX, finishY);

    int startX = int(auv->pos().x() / resolution);
    int startY = int(auv->pos().y() / resolution);

    start = new QPoint(startX, startY);

    return mapPoints;
}

void Map::AddPath(QVector<QPoint> pathPoints, const QColor &color)
{
    if (pathPoints.size() == 0)
        return;

    QPainterPath path;
    path.moveTo(pathPoints[0]);

    for(int i = 1; i < pathPoints.size(); i++)
    {
        path.lineTo(pathPoints[i]);
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





















