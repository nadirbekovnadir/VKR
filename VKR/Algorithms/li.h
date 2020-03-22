#ifndef LI_H
#define LI_H

#include <QPoint>
#include <QVector>

class Li
{
public:
    Li();

private:

    struct Point
    {
        Point(QPoint p)
        {
            x = p.x();
            y = p.y();
        }

        int x;
        int y;
    };
public:
    static qint64 wastedTime;

public:
    static bool CreatePath(const QVector<QVector<int>> &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path);
};

#endif // LI_H
