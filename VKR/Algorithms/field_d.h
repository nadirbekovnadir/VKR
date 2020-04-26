#ifndef FIELD_D_H
#define FIELD_D_H

#include <QVector>
#include <QPoint>
#include "Algorithms/custom_priority_queue.h"

class Field_D
{
public:
    Field_D();

public:
    struct PointNode
    {
        int x;
        int y;

        double g;
        double rhs;

        QVector<double> key = {0, 0};

        bool isInU = false;
        //bool isUpdated = false;

        const PointNode *cameFrom = nullptr;

        PointNode() {}
        PointNode(int x, int y) { this->x = x; this->y = y; }
        PointNode(const PointNode &node)
        {
            this->x = node.x;
            this->y = node.y;

            this->g = node.g;
            this->rhs = node.rhs;

            this->cameFrom = node.cameFrom;
        }

        void SetXY(int x, int y) { this->x = x; this->y = y; }

        bool operator ==(const PointNode &p) const { return ((this->x == p.x) && (this->y == p.y)); }
        bool operator !=(const PointNode &p) const { return ((this->x != p.x) || (this->y != p.y)); }
        bool operator <(const PointNode &p) const { return (key[0] < p.key[0]) || (key[0] == p.key[0] && key[1] < p.key[1]); }
        bool operator >(const PointNode &p) const { return (key[0] > p.key[0]) || (key[0] == p.key[0] && key[1] > p.key[1]); }
        bool operator <(const QVector<double> &k) const { return (key[0] < k[0]) || (key[0] == k[0] && key[1] < k[1]); }
        bool operator >(const QVector<double> &k) const { return (key[0] > k[0]) || (key[0] == k[0] && key[1] > k[1]); }
    };

    class MyCompare {
    public:
      bool operator()(Field_D::PointNode *a, Field_D::PointNode *b)
      {
        return (*a) > (*b);
      }
    };

    struct Point
    {
        int x;
        int y;
    };

public:
    static constexpr double inf = __DBL_MAX__;
    static constexpr double eps = __DBL_EPSILON__;

    enum PointNodeType
    {
        isInU = 0xA0,
        Reviewed = 0xA1
    };

    static qint64 wastedTime;

private:
    static PointNode *start;
    static PointNode *finish;

    static QPoint startP;
    static QPoint finishP;

    static bool isInitialized;
    static bool isMapUpdated;
    static bool isUSorted;

    static QVector<QVector<int>> map;
    static QVector<QVector<PointNode>> allPointNodes;

    static custom_priority_queue_comparer<PointNode*, MyCompare> U;
    static QVector<PointNode*> neighbors;
    static QVector<PointNode*> neighbors_2;
    //static custom_priority_queue<PointNode> reviewed;

    static double km;

public:
    static void SetMap(const QVector<QVector<int>> &map);
    static void SetFinishPoint(const QPoint &p);
    static void SetStartPoint(const QPoint &p);
    static void UpdateMap();
    static void ComputeShortestPath();
    static void UpdateVertex(PointNode *v);
    static QPoint MoveToNextPoint();
    static void CreatePath(QVector<QPoint> &pathPoints);
    static void Reset();
private:
    static void Initialize();
    static QVector<double> CalculateKey(const Field_D::PointNode *pn);

    static double H(const PointNode *pn);
    static double C(const PointNode *pn1, const PointNode *pn2);

    static void GetNeighbors(const PointNode *pn, bool second = false);

    static bool FirstKeyLess(const QVector<double> k1, const QVector<double> k2);
    static void SortU();
};

#endif // FIELD_D_H
