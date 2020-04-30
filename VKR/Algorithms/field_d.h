#ifndef FIELD_D_H
#define FIELD_D_H

#include <QVector>
#include <QPoint>
#include "Algorithms/custom_priority_queue.h"
#include <QSet>

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

        bool inOPEN = false;
        //bool isUpdated = false;

        //const PointNode *cameFrom = nullptr;
        PointNode *s_a = nullptr;
        PointNode *s_b = nullptr;

        PointNode() {}
        PointNode(int x, int y) { this->x = x; this->y = y; }
        PointNode(const PointNode &node)
        {
            this->x = node.x;
            this->y = node.y;

            this->g = node.g;
            this->rhs = node.rhs;

            //this->cameFrom = node.cameFrom;
            this->s_a = node.s_a;
            this->s_b = node.s_b;
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
        return *a > *b;
      }
    };

    static constexpr double inf = __DBL_MAX__;

private:
    static PointNode *start;
    static custom_priority_queue_comparer<PointNode*, MyCompare> OPEN;
    static QSet<PointNode> allNodes;

private:
    static QVector<double> Key(PointNode *pn);
    static double H(PointNode *pn);
    static void UpdateNode(PointNode *s);
    static double ComputeCost(PointNode *s, PointNode *s_1, PointNode *s_2);

public:
    static void ComputeShortestPath();

private:
    static constexpr int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    static constexpr int dy[8] = {0, -1, -1, -1, 0, 1, 1, 1};
    static PointNode* s_1;
    static PointNode* s_2;

private:
    static bool Nbrs_1(PointNode *s);
    static bool Nbrs_2(PointNode *s);
    static PointNode* FindInQSet(PointNode &pn);

private:
    //static constexpr int clock
    static PointNode temp_n;

private:
    static PointNode* Bptr(PointNode* s);
    static PointNode* Cknbr(PointNode* first, PointNode* second);
    static PointNode* Ccknbr(PointNode* first, PointNode* second);

};

uint qHash(const Field_D::PointNode &key) { return 100000000*key.x + key.y; }

#endif // FIELD_D_H
