#include "Utils.h"
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

int manhattanDistance(Node *a, Node *b)
{
    if (!a || !b)
        return 0;
    return (abs(a->x - b->x) + abs(a->y - b->y));
}

int calculateArrivalTime(Node *sinkNode)
{
    int time = 0;
    Node *current = sinkNode;
    while (current->parent != nullptr)
    {
        time += manhattanDistance(current, current->parent);
        current = current->parent;
    }
    return time;
}

int computeTotalWireLength(Node *root)
{
    if (!root)
        return 0;
    int totalLength = 0;
    queue<Node *> q;
    q.push(root);
    while (!q.empty())
    {
        Node *current = q.front();
        q.pop();
        for (Node *child : current->children)
        {
            totalLength += manhattanDistance(current, child);
            q.push(child);
        }
    }
    return totalLength;
}

pair<vector<Node *>, vector<Node *>> adaptivePartitionNodes(vector<Node *> &nodes)
{
    if (nodes.size() <= 1)
        return {nodes, {}};
    int minX = nodes[0]->x, maxX = nodes[0]->x;
    int minY = nodes[0]->y, maxY = nodes[0]->y;
    for (Node *node : nodes)
    {
        minX = min(minX, node->x);
        maxX = max(maxX, node->x);
        minY = min(minY, node->y);
        maxY = max(maxY, node->y);
    }
    bool splitX = (maxX - minX) >= (maxY - minY);
    if (splitX)
        sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
             { return a->x < b->x; });
    else
        sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
             { return a->y < b->y; });
    size_t mid = nodes.size() / 2;
    vector<Node *> left(nodes.begin(), nodes.begin() + mid);
    vector<Node *> right(nodes.begin() + mid, nodes.end());
    return {left, right};
}