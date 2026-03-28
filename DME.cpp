#include "DME.h"
#include "Utils.h"
#include "Overlap.h"
#include "Globals.h"
#include <iostream>
#include <algorithm>

using namespace std;

// --- Helper function declarations (internal to this module) ---
static void refineBufferPosition(Node *bufNode, Node *root, const map<string, Node*> &allBuffers, const Node *src, const vector<Node*> &sinks);
static void insertMidWireBuffers(Node* parent, Node* child, map<string, Node*> &allBuffers, Node* root, const Node *src, const vector<Node*> &sinks);

// --- DME Algorithm Implementation ---

MergeSubtree* bottomUpDME(vector<Node*> sinks) {
    if (sinks.empty()) return nullptr;
    if (sinks.size() == 1) return new MergeSubtree(sinks[0]);
    auto groups = adaptivePartitionNodes(sinks);
    MergeSubtree* left = bottomUpDME(groups.first);
    MergeSubtree* right = bottomUpDME(groups.second);
    if (!right) return left;

    int minX = max(left->region.minX, right->region.minX);
    int maxX = min(left->region.maxX, right->region.maxX);
    int minY = max(left->region.minY, right->region.minY);
    int maxY = min(left->region.maxY, right->region.maxY);

    if (minX > maxX) { minX = maxX = (left->region.minX + right->region.minX) / 2; }
    if (minY > maxY) { minY = maxY = (left->region.minY + right->region.minY) / 2; }

    FeasibleRegion region(minX, maxX, minY, maxY);
    return new MergeSubtree(region, {left, right});
}

void topDownEmbed(Node* parent, MergeSubtree* subtree, map<string, Node *> &allBuffers, Node* root, const Node *src, const vector<Node*> &sinks)
{
    if (!subtree) return;
    if (subtree->leaves.size() == 1 && subtree->children.empty()) {
        insertMidWireBuffers(parent, subtree->leaves[0], allBuffers, root, src, sinks);
        return;
    }

    int bufX = (subtree->region.minX + subtree->region.maxX) / 2;
    int bufY = (subtree->region.minY + subtree->region.maxY) / 2;

    auto pos = findNonOverlappingPosition(bufX, bufY, allBuffers, src, sinks);
    bufX = pos.first;
    bufY = pos.second;
    
    string bufId = "B" + to_string(++bufferCounter);
    Node* bufNode = new Node(bufId, bufX, bufY);
    allBuffers[bufId] = bufNode;
    
    insertMidWireBuffers(parent, bufNode, allBuffers, root, src, sinks);
    
    refineBufferPosition(bufNode, root, allBuffers, src, sinks);

    for (MergeSubtree* child : subtree->children) {
        topDownEmbed(bufNode, child, allBuffers, root, src, sinks);
    }
}

// --- Helper function definitions ---

void refineBufferPosition(Node *bufNode, Node *root, const map<string, Node*> &allBuffers, const Node *src, const vector<Node*> &sinks)
{
    if (!refineForTotalWireLength) return;

    static const int dx[] = {-tryMove, tryMove, 0, 0};
    static const int dy[] = {0, 0, -tryMove, tryMove};

    for (int iter = 0; iter < iterations; iter++)
    {
        int bestX = bufNode->x, bestY = bufNode->y;
        int bestLength = computeTotalWireLength(root);
        int oldX = bufNode->x, oldY = bufNode->y;

        for (int dir = 0; dir < 4; dir++)
        {
            int newX = oldX + dx[dir];
            int newY = oldY + dy[dir];
            if (newX < 0 || newX > dimX || newY < 0 || newY > dimY) continue;
            if (isOccupied(newX, newY, allBuffers, src, sinks)) continue;

            bufNode->x = newX; bufNode->y = newY;
            int wireLen = computeTotalWireLength(root);
            if (wireLen < bestLength)
            {
                bestLength = wireLen;
                bestX = newX;
                bestY = newY;
            }
        }

        bufNode->x = bestX; bufNode->y = bestY;
        if (bufNode->x == oldX && bufNode->y == oldY) break;
    }
}

void insertMidWireBuffers(Node* parent, Node* child, map<string, Node*> &allBuffers, Node* root, const Node *src, const vector<Node*> &sinks)
{
    int dist = manhattanDistance(parent, child);
    if (dist <= maxWireLength)
    {
        child->parent = parent;
        parent->children.push_back(child);
        return;
    }

    int segments = (dist + maxWireLength - 1) / maxWireLength;
    Node* lastNode = parent;
    int x1 = parent->x, y1 = parent->y;
    int x2 = child->x, y2 = child->y;

    for (int i = 1; i < segments; ++i)
    {
        double ratio = static_cast<double>(i) / segments;
        int bufX = static_cast<int>(x1 + ratio * (x2 - x1));
        int bufY = static_cast<int>(y1 + ratio * (y2 - y1));

        auto pos = findNonOverlappingPosition(bufX, bufY, allBuffers, src, sinks);
        bufX = pos.first; bufY = pos.second;

        string bufId = "B" + to_string(++bufferCounter);
        Node* bufNode = new Node(bufId, bufX, bufY);
        allBuffers[bufId] = bufNode;
        bufNode->parent = lastNode;
        lastNode->children.push_back(bufNode);
        lastNode = bufNode;
    }

    child->parent = lastNode;
    lastNode->children.push_back(child);
}