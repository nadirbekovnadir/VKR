#include "a_star.h"
#include <QMap>
#include <QtMath>
#include <QList>
#include "map.h"

A_Star::A_Star()
{

}

QVector<QVector<int>> A_Star::map;

bool A_Star::CreatePath(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path)
{
    A_Star::map = map;

    PointNode start(startP.x(), startP.y());
    PointNode finish(stopP.x(), stopP.y());

    QMultiMap<double, PointNode> open;
    QList<PointNode> closed;

    start.g = 0;
    start.h = Heuristic(start, finish);
    start.f = start.g + start.h;

    open.insert(start.f, start);

    PointNode current;
    QVector<PointNode> neighbors;
    neighbors.reserve(8);

    bool isInOpen = false;
    bool isScoreBetter = false;
    double gScore = 0;

    while (open.size() != 0)
    {
        current = open.first();

        if (current == finish)
        {
            path = ReconstructPath(current);
            return true;
        }

        open.remove(current.f, current);

        neighbors.clear();
        closed.append(current);

        GetNeighbors(neighbors, closed, current);

        for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
        {
            isInOpen = open.contains(neighbor->f, *neighbor);

            gScore = current.g + DistanceBetween(*neighbor, current);

            if (!isInOpen)
            {
                neighbor->g = gScore;
                neighbor->h = Heuristic(*neighbor, finish);
                neighbor->f = neighbor->g + neighbor->h;

                neighbor->cameFrom = &closed.at(closed.size() - 1);

                open.insert(neighbor->f, *neighbor);

                continue;
            }

            isScoreBetter = gScore < neighbor->g;
            if (isScoreBetter)
            {
                neighbor->g = gScore;
                neighbor->cameFrom = &closed.at(closed.size() - 1);

                open.remove(neighbor->f, *neighbor);

                neighbor->f = neighbor->g + neighbor->h;
                open.insert(neighbor->f, *neighbor);
            }

        }

    }

    return true;
}

double A_Star::DistanceBetween(const A_Star::PointNode &first, const A_Star::PointNode &second)
{
    return qSqrt(qPow(first.x - second.x, 2) + qPow(first.y - second.y, 2));
}

double A_Star::Heuristic(const A_Star::PointNode &p, const A_Star::PointNode &finish)
{
    return qSqrt(qPow(p.x - finish.x, 2) + qPow(p.y - finish.y, 2));
}

void A_Star::GetNeighbors(QVector<PointNode> &neighbors, const QList<PointNode> &closed, const PointNode &p)
{
    PointNode tempNode;
    for (int x = p.x - 1; x <= p.x + 1; x++)
        for (int y = p.y - 1; y <= p.y + 1; y++)
        {
            tempNode.SetXY(x, y);

            if (closed.contains(tempNode))
                continue;

            if (x == p.x && y == p.y)
                continue;

            if (x < 0 || y < 0 || x > map.size() || y > map[0].size())
                continue;

            if (map[x][y] == Map::DotType::obstacle)
                continue;

            neighbors.push_back(tempNode);
        }
}

QVector<QPoint> A_Star::ReconstructPath(const A_Star::PointNode &finish)
{
    const PointNode *tempNode = &finish;
    QVector<QPoint> path;

    path.append(QPoint(tempNode->x, tempNode->y));

    while (tempNode->cameFrom != nullptr)
    {
        tempNode = tempNode->cameFrom;
        path.append(QPoint(tempNode->x, tempNode->y));
    }

    return path;
}
