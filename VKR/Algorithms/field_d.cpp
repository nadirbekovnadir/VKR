#include "field_d.h"
#include <QtMath>

Field_D::PointNode *Field_D::start;
custom_priority_queue_comparer<Field_D::PointNode*, Field_D::MyCompare> Field_D::OPEN;
QSet<Field_D::PointNode> Field_D::allNodes;

QVector<double> Field_D::Key(Field_D::PointNode *pn)
{
    QVector<double> k(2);
    k[1] = qMin(pn->g, pn->rhs);
    k[0] = k[1] + H(pn);
    return k;
}

double Field_D::H(Field_D::PointNode *pn)
{
    return qSqrt(qPow(pn->x - start->x, 2) + qPow(pn->y - start->y, 2));
}

void Field_D::UpdateNode(Field_D::PointNode *pn)
{
    if (pn->g != pn->rhs)
    {
        pn->key = Key(pn);
        OPEN.push(pn);
        pn->inOPEN = true;
    }
    else if (pn->inOPEN)
    {
        OPEN.remove(pn);
        pn->inOPEN = false;
    }
}

double Field_D::ComputeCost(Field_D::PointNode *s, Field_D::PointNode *s_1, Field_D::PointNode *s_2)
{
    return 0; //
}

void Field_D::ComputeShortestPath()
{
    PointNode *s = OPEN.top();
    double rhs_new;
    double rhs_old;
    double temp_cost;
    PointNode *bptr_node;
    PointNode *temp_node_too;

    while (*s < Key(start) || start->g != start->rhs)
    {
        if (s->g >= s->rhs)
        {
            s->g = s->rhs;
            OPEN.pop();
            s->inOPEN = false;

            while (Nbrs_1(s))
            {
                rhs_old = s_1->rhs;

                temp_cost = ComputeCost(s_1, s, Ccknbr(s_1, s));
                if (s_1->rhs > temp_cost)
                {
                    s_1->rhs = temp_cost;
                    bptr_node = Bptr(s_1);
                    bptr_node = s;
                }

                temp_node_too = Cknbr(s_1, s);
                temp_cost = ComputeCost(s_1, s, temp_node_too);
                if (s_1->rhs > temp_cost)
                {
                    s_1->rhs = temp_cost;
                    bptr_node = Bptr(s_1);
                    bptr_node = temp_node_too;
                }

                if (rhs_old != s->rhs)
                    UpdateNode(s_1);
            }
        }
        else
        {
            s->rhs = inf;
            bptr_node = Bptr(s);
            while (Nbrs_1(s))
            {
                rhs_new = ComputeCost(s, s_1, Ccknbr(s, s_1));
                if (rhs_new < s->rhs)
                {
                    s->rhs = rhs_new;
                    //bprt_node =
                }
            }

            if (s->g < s->rhs)
            {
                s->g = inf;
                while (Nbrs_1(s))
                {
                    bptr_node = Bptr(s_1);
                    bptr_node = Bptr(s);
                    if (*bptr_node == *s || bptr_node ==Cknbr(s_1, s))
                    {   if (s_1->rhs != ComputeCost(s_1, bptr_node, Ccknbr(s_1, bptr_node)))
                        {   if (s_1->g < s_1->rhs || !s_1->inOPEN)
                            {
                                s_1->rhs = inf;
                                UpdateNode(s_1);
                            }
                            else
                            {
                                s_1->rhs = inf;
                                while (Nbrs_1(s))
                                {
                                    rhs_new = ComputeCost(s_1, s_2, Ccknbr(s_1, s_2));
                                    if (rhs_new < s->rhs)
                                    {
                                        s->rhs = rhs_new;
                                        //bprt_node =
                                    }
                                }
                                UpdateNode(s_1);
                            }
                        }
                    }
                }
            }
        }
        UpdateNode(s);
    }
}


int const Field_D::dx[8];
int const Field_D::dy[8];
Field_D::PointNode* Field_D::s_1;
Field_D::PointNode* Field_D::s_2;

bool Field_D::Nbrs_1(Field_D::PointNode *s)
{
    static int count = 0;
    static PointNode temp_s;

    if (count == 8)
    {
        count = 0;
        return false;
    }

    temp_s.SetXY(s->x + dx[count], s->y + dy[count]);
    if (!allNodes.contains(temp_s))
    {
        temp_s.g = inf;
        temp_s.rhs = inf;

        allNodes.insert(temp_s);
    }
    s_1 = const_cast<PointNode*>(&(*allNodes.find(temp_s)));

    count++;
    return true;
}

bool Field_D::Nbrs_2(Field_D::PointNode *s)
{
    static int count = 0;
    static PointNode temp_s;

    if (count == 8)
    {
        count = 0;
        return false;
    }

    temp_s.SetXY(s->x + dx[count], s->y + dy[count]);
    s_2 = FindInQSet(temp_s);

    count++;
    return true;
}

Field_D::PointNode *Field_D::FindInQSet(Field_D::PointNode &pn)
{
    if (!allNodes.contains(pn))
    {
        pn.g = inf;
        pn.rhs = inf;

        allNodes.insert(pn);
    }
    return const_cast<PointNode*>(&(*allNodes.find(pn)));
}

Field_D::PointNode *Field_D::Bptr(Field_D::PointNode *s)
{
    if (s->s_a == nullptr)
        return s->s_b;

    if (s->s_b == nullptr)
        return s->s_a;

    if ((s->s_a == nullptr) && (s->s_b == nullptr))
        return s->s_a;

    if (s->s_b->x == s->s_a->x)
    {
        if (s->s_b->y - s->s_a->y > 0)
        {
            if (s->s_b->x - s->x > 0)
                return s->s_b;
            else
                return s->s_a;
        }
        else
        {
            if (s->s_b->x - s->x > 0)
                return s->s_a;
            else
                return s->s_b;
        }
    }

    if (s->s_b->x - s->s_a->x > 0)
    {
        if (s->s_b->y - s->y > 0)
            return s->s_a;
        else
            return s->s_b;
    }
    else
    {
        if (s->s_b->y - s->y > 0)
            return s->s_b;
        else
            return s->s_a;
    }
}

Field_D::PointNode Field_D::temp_n;

Field_D::PointNode *Field_D::Cknbr(Field_D::PointNode *first, Field_D::PointNode *second)
{
    if (second->x == first->x)
    {
        if (second->y - first->y > 0)
            temp_n.SetXY(second->x - 1, second->y);
        else
            temp_n.SetXY(second->x + 1, second->y);
    }

    if (second->x - first->x > 0)
    {
        if (second->y - first->y > 0)
            temp_n.SetXY(second->x - 1, second->y);
        else
            temp_n.SetXY(second->x, second->y + 1);
    }
    else
    {
        if (first->y - second->y > 0)
            temp_n.SetXY(second->x + 1, second->y);
        else
            temp_n.SetXY(second->x, second->y - 1);
    }

    return FindInQSet(temp_n);
}

Field_D::PointNode *Field_D::Ccknbr(Field_D::PointNode *first, Field_D::PointNode *second)
{
    if (second->x == first->x)
    {
        if (second->y - first->y > 0)
            temp_n.SetXY(second->x + 1, second->y);
        else
            temp_n.SetXY(second->x - 1, second->y);
    }

    if (second->x - first->x > 0)
    {
        if (second->y - first->y < 0)
            temp_n.SetXY(second->x - 1, second->y);
        else
            temp_n.SetXY(second->x, second->y - 1);
    }
    else
    {
        if (first->y - second->y < 0)
            temp_n.SetXY(second->x + 1, second->y);
        else
            temp_n.SetXY(second->x, second->y + 1);
    }

    return FindInQSet(temp_n);
}














