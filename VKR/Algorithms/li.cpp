#include "li.h"
#include "map.h"

Li::Li()
{

}

bool Li::CreatePath(const QVector<QVector<int> > &map, const QPoint &startP, const QPoint &stopP, QVector<QPoint> &path)
{
    QVector<QVector<int>> grid = map;

    int mapWidth = grid.size();
    int mapHeight = grid[0].size();

    Point start(startP);
    Point stop(stopP);

    bool isMarked;

    if (grid[start.x][start.y] == Map::DotType::obstacle || grid[stop.x][stop.y] == Map::DotType::obstacle)
        return false;

    int x, y, d, k;

    const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    int ix;
    int iy;

    d = 0;
    grid[start.x][start.y] = d;
    do
    {
        isMarked = true;
        for (x = 0; x < mapWidth; x++)
            for (y = 0; y < mapHeight; y++)
            {

                if (grid[x][y] == d)
                    for (k = 0; k < 8; k++)
                    {
                        ix = x + dx[k];
                        iy = y + dy[k];

                        if (ix >= 0 && ix < mapWidth &&
                            iy >= 0 && iy < mapHeight &&
                            grid[ix][iy] == Map::DotType::passable)
                        {
                            isMarked = false;
                            grid[ix][iy] = d + 1;
                        }
                    }

            }
        d++;
    }
    while (!isMarked && grid[stop.x][stop.y] < 0);

    if (grid[stop.x][stop.y] < 0)
        return false;



    x = stop.x;
    y = stop.y;
    d = grid[x][y];

    while (d > 0)
    {
        path.append(QPoint(x, y));
        d--;

        for (k = 0; k < 8; k++)
        {
            ix = x + dx[k];
            iy = y + dy[k];

            if (ix >= 0 && ix < mapWidth &&
                iy >= 0 && iy < mapHeight &&
                grid[ix][iy] == d)
            {
                x = ix;
                y = iy;

                break;
            }
        }
    }

    path.append(QPoint(start.x, start.y));

    return true;
}
