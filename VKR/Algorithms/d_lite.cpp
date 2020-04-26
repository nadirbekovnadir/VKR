#include "d_lite.h"
#include <limits.h>
#include <QtMath>
#include "map.h"

D_Lite::D_Lite()
{
    
}

void D_Lite::SetMap(const QVector<QVector<int> > &map)
{
    D_Lite::map = map;
}

void D_Lite::SetFinishPoint(const QPoint &p)
{
    finishP = p;
}

void D_Lite::SetStartPoint(const QPoint &p)
{
    startP = p;
}

qint64 D_Lite::wastedTime;

D_Lite::PointNode *D_Lite::start;
D_Lite::PointNode *D_Lite::finish;

QPoint D_Lite::startP;
QPoint D_Lite::finishP;

bool D_Lite::isInitialized = false;
bool D_Lite::isMapUpdated = false;
bool D_Lite::isUSorted = true;

QVector<QVector<int>> D_Lite::map;
QVector<QVector<D_Lite::PointNode>> D_Lite::allPointNodes;

custom_priority_queue_comparer<D_Lite::PointNode*, D_Lite::MyCompare> D_Lite::U;
QVector<D_Lite::PointNode*> D_Lite::neighbors;
QVector<D_Lite::PointNode*> D_Lite::neighbors_2;

double D_Lite::km = 0;

void D_Lite::ComputeShortestPath()
{
    QElapsedTimer timer;
    timer.start();

    // Проверим, была ли уже выполнена инициализация
    if (!isInitialized)
        Initialize();

    // Создание переменных для хранения временных параметров
    PointNode *u;
    QVector<double> k_old;
    QVector<double> k_new;
    double g_old;

    PointNode *neighbor;
    // // //

    if (!isUSorted)
        SortU();

    u = U.top(); // инициализируем рассматриваемый узел
    // Главный цикл, проверяются условия достижения цели, либо отсутствия возможности ее достижения
    while(*u < CalculateKey(start) || start->rhs > start->g) // использую другую перегрузку сравнения
    {
        if (!isUSorted)
            SortU();

        u = U.top(); // Хоть и дублирование, но не забуду это выполнить
        k_old = u->key; // старый ключ
        k_new = CalculateKey(u); // новый ключ

        // Если новый ключ больше старого, требуется обновить его в очереди
        if (FirstKeyLess(k_old, k_new))
        {
            u->key = k_new;
            U.pop();
            U.push(u);
        }
        // // //

        // Если узел не обновлен в соотв с rhs, обновим и удалим из очереди (он стал рассмотренным)
        else if (u->g > u->rhs)
        {
            u->g = u->rhs;
            U.pop();
            u->isInU = false;

            // Обновим значения rhs для соседей узла
            GetNeighbors(u);
            for (int s = 0; s < neighbors.size(); s++)
            {
                neighbor = neighbors[s];
                if (*neighbor != *finish)
                    neighbor->rhs = qMin(neighbor->rhs, C(neighbor, u) + u->g);
                UpdateVertex(neighbor); // обновим узел
            }
            // // //

        }
        // // //

        // если rhs > g, значит, есть вероятность, что rhs соседей будет повышен
        else
        {
            g_old = u->g;
            u->g = inf;

            // Обновим rhs всех соседей узла, включая сам узел
            GetNeighbors(u);
            for (int s = 0; s < neighbors.size(); s++)
            {
                neighbor = neighbors[s];
                if (s == neighbors.size() - 1)
                    neighbor = u;

                if (qAbs(neighbor->rhs - (C(neighbor, u) + g_old)) < eps)
                    if (*neighbor != *finish)
                    {
                        GetNeighbors(neighbor, true);

                        for (int s2 = 0; s2 < neighbors_2.size(); s2++)
                            neighbor->rhs = qMin(neighbor->rhs, C(neighbor, neighbors_2[s2]) + neighbors_2[s2]->g);
                    }

                UpdateVertex(neighbor);
            }
            // // //
        }
    }

    wastedTime = timer.elapsed();
}

void D_Lite::UpdateVertex(D_Lite::PointNode *v)
{
    // Если узел локально неустойчив, добавим его в очередь, либо обновим ключ в ней
    if (qAbs(v->g - v->rhs) > eps)
    {
        if (v->isInU)
        {
            U.remove(v);
        }

        v->isInU = true;
        v->key = CalculateKey(v);
        U.push(v);
        isUSorted = true;
    }
    // Если локально устойчив и находится в очереди, удалим его из нее
    else if (qAbs(v->g - v->rhs) <= eps && v->isInU)
    {
        v->isInU = false;
        U.remove(v);
        isUSorted = false;

    }
}

void D_Lite::CreatePath(QVector<QPoint> &pathPoints)
{
    double g = start->g;
    pathPoints << QPoint(start->x, start->y);

    PointNode *p = start;
    while (*p != *finish)
    {
        GetNeighbors(p);
        for (int s = 0; s < neighbors.size(); s++)
        {
            if (neighbors[s]->g < g)
            {
                g = neighbors[s]->g;
                p = neighbors[s];
            }
        }
        pathPoints << QPoint(p->x, p->y);
    }
}

void D_Lite::Reset()
{
    isInitialized = false;
}

void D_Lite::Initialize()
{
    isUSorted = true;

    while(!U.empty()) // Очистим очередь вершин, которые надо рассмотреть
        U.pop();

    km = 0;

    // Начало инициализации хранилища всех вершин
    allPointNodes.clear();
    allPointNodes.resize(map.size());
    for (int i = 0; i < allPointNodes.size(); i++)
        allPointNodes[i].resize(map[i].size());

    PointNode tempNode;
    for (int x = 0; x < allPointNodes.size(); x++)
        for (int y = 0; y < allPointNodes[0].size(); y++)
        {
            tempNode.SetXY(x, y);
            tempNode.g = inf;
            tempNode.rhs = inf;
            allPointNodes[x][y] = tempNode;
        }
    // // //

    // Инициализация указателей на узлы финиша и старта
    start = &allPointNodes[startP.x()][startP.y()];
    finish = &allPointNodes[finishP.x()][finishP.y()];
    // // //

    // Определние параметров финишного узла и добавление его в очередь
    finish->g = inf;
    finish->rhs = 0;
    finish->key = CalculateKey(finish);
    finish->isInU = true;

    U.push(finish);
    // // //

    // Выставка флага завершенной инициализации
    isInitialized = true;
}

QVector<double> D_Lite::CalculateKey(const PointNode *pn)
{
    QVector<double> key(2);
    key[0] = qMin(pn->g, pn->rhs) + H(pn) + km;
    key[1] = qMin(pn->g, pn->rhs);
    return key;
}

double D_Lite::H(const D_Lite::PointNode *pn)
{
    return qSqrt(qPow(pn->x - start->x, 2) + qPow(pn->y - start->y, 2));
}

double D_Lite::C(const D_Lite::PointNode *pn1, const D_Lite::PointNode *pn2)
{
    return qSqrt(qPow(pn1->x - pn2->x, 2) + qPow(pn1->y - pn2->y, 2));
}

void D_Lite::GetNeighbors(const PointNode *pn, bool second)
{
    if (!second)
        neighbors.clear();
    else
        neighbors_2.clear();

    int pointType;

    for (int x = pn->x - 1; x <= pn->x + 1; x++)
        for (int y = pn->y - 1; y <= pn->y + 1; y++)
        {
            if (x < 0 || y < 0 || x >= map.size() || y >= map[0].size())
                continue;

            pointType = map[x][y];

//            if (pointType == PointNodeType::inClosed)
//                continue;

            if (pointType == Map::DotType::obstacle)
                continue;

            if (!second)
                neighbors.push_back(&allPointNodes[x][y]);
            else
                neighbors_2.push_back(&allPointNodes[x][y]);
        }
}

bool D_Lite::FirstKeyLess(const QVector<double> k1, const QVector<double> k2)
{
    return (k1[0] < k2[0]) || (k1[0] == k2[0] && k1[1] < k2[1]);
}

void D_Lite::SortU()
{
    PointNode n;
    n.key[0] = -1;
    U.push(&n); // Необходимо, чтобы снова выстроить требуемый порядок очереди
    U.pop();

    isUSorted = true;
}


