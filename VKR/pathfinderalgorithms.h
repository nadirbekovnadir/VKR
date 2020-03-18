#ifndef PATHFINDERALGORITHMS_H
#define PATHFINDERALGORITHMS_H

#include <QPoint>
#include <QVector>

class PathfinderAlgorithms
{
public:
    PathfinderAlgorithms();

public:
    bool A_star(const QVector<QVector<int>> &map, const QPoint &start, const QPoint &finish, QVector<QPoint> &path);
private:
    struct PointNode
    {
        QPoint value;
        PointNode *parent = nullptr;

        double g;
        double h;
        double f;

        bool operator==(const PointNode &p)
        { return this->value == p.value; }
    };

    PointNode *start;
    PointNode *finish;

    QVector<QVector<int>> map;

    double DistanceBetween(const PointNode *x, const PointNode *y);
    double Heuristic_A_star(const PointNode *p);

    QVector<PointNode*> FindNeighbors(const PointNode *p);

    QVector<QPoint> ReconstructPath(const PointNode *start, const PointNode *finish);
};

#endif // PATHFINDERALGORITHMS_H
