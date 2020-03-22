#include "a_star.h"
#include <QtMath>
#include "map.h"

A_Star::A_Star()
{

}

QVector<QVector<int>> A_Star::map;

A_Star::PointNode A_Star::start;
A_Star::PointNode A_Star::finish;

custom_priority_queue<A_Star::PointNode> A_Star::open;
QList<A_Star::PointNode> A_Star::closed;

QVector<A_Star::PointNode> A_Star::neighbors;

bool A_Star::CreatePath(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path)
{
    A_Star::map = map;

    start.SetXY(startP.x(), startP.y());
    finish.SetXY(stopP.x(), stopP.y());

    open = custom_priority_queue<PointNode>();
    closed.clear();

    start.g = 0;
    start.h = Heuristic(start);
    start.f = start.g + start.h;

    open.push(start);
    A_Star::map[start.x][start.y] = PointNodeType::inOpened;

    PointNode current;
    neighbors.reserve(8);

    double gScore = 0;

    while (open.size() != 0)
    {
        current = open.top();
        open.pop();

        if (current == finish)
        {
            finish = current;
            path = ReconstructPath();
            return true;
        }

        neighbors.clear();
        closed.append(current);
        A_Star::map[current.x][current.y] = PointNodeType::inClosed;

        GetNeighbors(current);

        for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
        {

            gScore = current.g + DistanceBetween(*neighbor, current);

            if (A_Star::map[neighbor->x][neighbor->y] != PointNodeType::inOpened)
            {
                neighbor->g = gScore;
                neighbor->h = Heuristic(*neighbor);
                neighbor->f = neighbor->g + neighbor->h;

                neighbor->cameFrom = &closed.last();

                open.push(*neighbor);
                A_Star::map[neighbor->x][neighbor->y] = PointNodeType::inOpened;

                continue;
            }

            if (gScore < neighbor->g)
            {
                open.remove(*neighbor);

                neighbor->g = gScore;
                neighbor->cameFrom = &closed.last();
                neighbor->f = neighbor->g + neighbor->h;

                open.push(*neighbor);
            }

        }

    }

    return true;
}

double A_Star::DistanceBetween(const A_Star::PointNode &first, const A_Star::PointNode &second)
{
    return qSqrt(qPow(first.x - second.x, 2) + qPow(first.y - second.y, 2));
}

double A_Star::Heuristic(const A_Star::PointNode &p)
{
    return qSqrt(qPow(p.x - finish.x, 2) + qPow(p.y - finish.y, 2));
}

void A_Star::GetNeighbors(const PointNode &p)
{
    PointNode tempNode;
    int pointType;

    for (int x = p.x - 1; x <= p.x + 1; x++)
        for (int y = p.y - 1; y <= p.y + 1; y++)
        {
            if (x < 0 || y < 0 || x >= map.size() || y >= map[0].size())
                continue;

            pointType = map[x][y];

            if (pointType == PointNodeType::inClosed)
                continue;

            if (pointType == Map::DotType::obstacle)
                continue;

            tempNode.SetXY(x, y);
            neighbors.push_back(tempNode);
        }
}

QVector<QPoint> A_Star::ReconstructPath()
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
