#ifndef A_STAR_H
#define A_STAR_H

#include <QPoint>
#include <QVector>
#include <QList>
#include <Algorithms/custom_priority_queue.h>

using namespace std;

class A_Star
{
public:
    A_Star();

public:
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
        bool operator <(const PointNode &p) const { return this->f > p.f; }
    };

    enum PointNodeType
    {
        inClosed = 10,
        inOpened = 11,
        undefined = 12,
    };
public:
    static qint64 wastedTime;

private:
    static QVector<QVector<int>> map;

    static PointNode start;
    static PointNode finish;

    static custom_priority_queue<PointNode> open;
    static QList<PointNode> closed;

    static QVector<PointNode> neighbors;

public:
    static bool CreatePath(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path);

private:
    static double DistanceBetween(const PointNode &first, const PointNode &second);
    static double Heuristic(const PointNode &p);

    static void GetNeighbors(const PointNode &p);

    static QVector<QPoint> ReconstructPath();
};

#endif // A_STAR_H
