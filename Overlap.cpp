#include "Overlap.h"
#include "Globals.h"
#include <iostream>
#include <cmath>

using namespace std;

bool isOccupied(int x, int y, const map<string, Node*> &allBuffers, const Node *src, const vector<Node*> &sinks)
{
    if (src && src->x == x && src->y == y) return true;
    for (const auto &p : allBuffers)
    {
        if (p.second->x == x && p.second->y == y) return true;
    }
    for (const Node *sink : sinks)
    {
        if (sink->x == x && sink->y == y) return true;
    }
    return false;
}

pair<int, int> findNonOverlappingPosition(int originalX, int originalY, const map<string, Node*> &allBuffers, const Node *src, const vector<Node*> &sinks, int maxJitter)
{
    if (!isOccupied(originalX, originalY, allBuffers, src, sinks))
        return {originalX, originalY};

    for (int jitterRadius = 1; jitterRadius <= maxJitter; ++jitterRadius)
    {
        for (int dx = -jitterRadius; dx <= jitterRadius; ++dx)
        {
            for (int dy = -jitterRadius; dy <= jitterRadius; ++dy)
            {
                if (abs(dx) != jitterRadius && abs(dy) != jitterRadius) continue;
                int candidateX = originalX + dx;
                int candidateY = originalY + dy;
                if (candidateX < 0 || candidateX > dimX || candidateY < 0 || candidateY > dimY) continue;
                if (!isOccupied(candidateX, candidateY, allBuffers, src, sinks))
                    return {candidateX, candidateY};
            }
        }
    }

    cerr << "Warning: Unable to find non-overlapping position near (" << originalX << ", " << originalY << ")\n";
    return {originalX, originalY};
}

bool checkOverlapping(const map<string, Node*> &allBuffers, const Node *src, const vector<Node*> &sinks)
{
    map<pair<int,int>, string> occupiedPositions;
    occupiedPositions[{src->x, src->y}] = src->id;

    for (const auto &p : allBuffers)
    {
        auto pos = make_pair(p.second->x, p.second->y);
        if (occupiedPositions.count(pos))
        {
            cerr << "Overlapping detected between " << occupiedPositions[pos] << " and " << p.second->id << " at (" << pos.first << ", " << pos.second << ")\n";
            return true;
        }
        occupiedPositions[pos] = p.second->id;
    }

    for (const Node *sink : sinks)
    {
        auto pos = make_pair(sink->x, sink->y);
        if (occupiedPositions.count(pos))
        {
            cerr << "Overlapping detected between " << occupiedPositions[pos] << " and " << sink->id << " at (" << pos.first << ", " << pos.second << ")\n";
            return true;
        }
        occupiedPositions[pos] = sink->id;
    }
    return false;
}