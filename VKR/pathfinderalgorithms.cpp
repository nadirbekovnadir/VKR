#include "pathfinderalgorithms.h"
#include <QList>
#include <QMap>
#include <QtMath>
#include <map.h>

PathfinderAlgorithms::PathfinderAlgorithms()
{

}

bool PathfinderAlgorithms::A_star(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &finishP, QVector<QPoint> &path)
{
    if (map.size() == 0)
        return false;
    else if (map.first().size() == 0)
        return false;

    start = new PointNode();
    finish = new PointNode();

    start->value = startP;
    finish->value = finishP;

    this->map = map;

    QList<PointNode*> closed;
    QMultiMap<double, PointNode*> open;

    start->g = 0;
    start->h = Heuristic_A_star(start);
    start->f = start->g + start->h;

    open.insert(start->f, start);

    PointNode *current;
    QVector<PointNode*> neighbors;
    double tentative_g_score;
    bool tentative_is_better;

    while (open.size() != 0)
    {
        current = open.first();
        if (current->value == finish->value)
        {
            path = ReconstructPath(start, current);

            for (auto iter = open.begin(); iter != open.end(); iter++)
                delete *iter;

            for (auto iter = closed.begin(); iter != closed.end(); iter++)
                delete *iter;

            return true;
        }

        open.remove(open.firstKey());
        closed.append(current);

        neighbors.clear();
        neighbors = FindNeighbors(current);
        for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
        {
            if (closed.contains(*neighbor))
                continue;

            tentative_g_score = current->g + DistanceBetween(current, *neighbor);

            bool isContains = open.contains((*neighbor)->f);
            if (tentative_g_score < (*neighbor)->g || !isContains)
                tentative_is_better = true;
            else
                tentative_is_better = false;

            if (tentative_is_better)
            {
                (*neighbor)->parent = current;
                (*neighbor)->g = tentative_g_score;
                (*neighbor)->h = Heuristic_A_star(*neighbor);
                (*neighbor)->f = (*neighbor)->g + (*neighbor)->h;
            }

            if (!isContains)
            {
                open.insert((*neighbor)->f, *neighbor);
            }
        }
    }

    for (auto iter = open.begin(); iter != open.end(); iter++)
        delete *iter;

    for (auto iter = closed.begin(); iter != closed.end(); iter++)
        delete *iter;

    return false;
}

double PathfinderAlgorithms::DistanceBetween(const PathfinderAlgorithms::PointNode *x, const PathfinderAlgorithms::PointNode *y)
{
    return qSqrt(qPow(x->value.x() - y->value.x(), 2) + qPow(x->value.y() - y->value.y(), 2));
}

double PathfinderAlgorithms::Heuristic_A_star(const PathfinderAlgorithms::PointNode *p)
{
    return DistanceBetween(p, finish);
}

QVector<PathfinderAlgorithms::PointNode*> PathfinderAlgorithms::FindNeighbors(const PathfinderAlgorithms::PointNode *p)
{
    QVector<PointNode*> neighbors;
    PointNode *tempNode;

    for (int x = p->value.x() - 1; x <= p->value.x() + 1; x++)
        for (int y = p->value.y() - 1; y <= p->value.y() + 1; y++)
        {
            if (x < 0 || y < 0 || x > map.size() - 1 || y > map.first().size() - 1)
                continue;

            if (x == p->value.x() && y == p->value.y())
                continue;

            if (map[x][y] == Map::DotType::passable)
            {
                tempNode = new PointNode();
                tempNode->value.setX(x);
                tempNode->value.setY(y);
                neighbors.append(tempNode);
            }
        }

    return neighbors;
}

QVector<QPoint> PathfinderAlgorithms::ReconstructPath(const PathfinderAlgorithms::PointNode *start, const PathfinderAlgorithms::PointNode *finish)
{
    QVector<QPoint> path;

    const PointNode *current = finish;

    while (current != nullptr)
    {
        path.append(current->value);
        current = current->parent;
    }

    return path;
}




















