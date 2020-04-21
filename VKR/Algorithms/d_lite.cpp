#include "d_lite.h"

D_Lite::D_Lite()
{

}

D_Lite::PointNode D_Lite::start(0, 0);
D_Lite::PointNode D_Lite::finish(0, 0);

bool D_Lite::isInitialized = false;
bool D_Lite::isMapUpdated = false;

QVector<QVector<int>> D_Lite::map;
custom_priority_queue<PointNode> U;

void D_Lite::ComputeShortestPath()
{
    if (!isInitialized)
        Initialize();
}

void D_Lite::Initialize()
{

}


