#ifndef D_LITE_H
#define D_LITE_H

#include <QVector>
#include <QPoint>
#include "Algorithms/custom_priority_queue.h"

class D_Lite
{
public:
    D_Lite();

public:
    struct PointNode
    {
        int x;
        int y;

        double g;
        double h;
        double rhs;

        //Add k1 and k2

        const PointNode *cameFrom = nullptr;

        PointNode() {}
        PointNode(int x, int y) { this->x = x; this->y = y; }
        PointNode(const PointNode &node)
        {
            this->x = node.x;
            this->y = node.y;

            this->g = node.g;
            this->h = node.h;
            this->rhs = node.rhs;

            this->cameFrom = node.cameFrom;
        }

        void SetXY(int x, int y) { this->x = x; this->y = y; }

        bool operator ==(const PointNode &p) const { return ((this->x == p.x) && (this->y == p.y)); }
        //bool operator <(const PointNode &p) const { return this->f > p.f; } //Add new <
    };

private:
    static PointNode start;
    static PointNode finish;

    static bool isInitialized;
    static bool isMapUpdated;

    static QVector<QVector<int>> map;
    static custom_priority_queue<PointNode> U;

public:
    static void SetMap(const QVector<QVector<int>> &map);
    static void SetFinishPoint(QPoint p);
    static void SetStartPoint(QPoint p);
    static void UpdateMap();
    static void ComputeShortestPath();
    static QPoint MoveToNextPoint();
private:
    static void Initialize();
    static void UpdateVertex();
    static void CalculateKey();

};

#endif // D_LITE_H
