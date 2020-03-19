#ifndef A_STAR_H
#define A_STAR_H

#include <QPoint>
#include <QVector>



class A_Star
{
public:
    A_Star();

public:
    static QVector<QVector<int>> map;

    static bool CreatePath(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path);

    struct PointNode
    {
        int x;
        int y;

        double g;
        double h;
        double f;

        const PointNode *cameFrom = nullptr;

        PointNode() {}
        PointNode(int x, int y) { this->x = x; this->y = y; }
        PointNode(const PointNode &node)
        {
            this->x = node.x;
            this->y = node.y;

            this->g = node.g;
            this->h = node.h;
            this->f = node.f;

            this->cameFrom = node.cameFrom;
        }

        void SetXY(int x, int y) { this->x = x; this->y = y; }

        bool operator ==(const PointNode &p) const { return ((this->x == p.x) && (this->y == p.y)); }
    };

    static double DistanceBetween(const PointNode &first, const PointNode &second);
    static double Heuristic(const PointNode &p, const A_Star::PointNode &finish);

    static void GetNeighbors(QVector<PointNode> &neighbors, const QList<PointNode> &closed, const PointNode &p);

    static QVector<QPoint> ReconstructPath(const PointNode &finish);
};

#endif // A_STAR_H
