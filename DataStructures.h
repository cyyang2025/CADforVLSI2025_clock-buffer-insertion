#pragma once

#include <string>
#include <vector>
#include <map>

// Represents a node in the clock tree (source, sink, or buffer)
struct Node
{
    std::string id;
    int x, y;
    Node *parent = nullptr;
    std::vector<Node *> children;
    Node(std::string _id, int _x, int _y) : id(_id), x(_x), y(_y) {}
};

// Holds the initial data parsed from the input file
struct ParsedData
{
    Node *src = nullptr;
    std::vector<Node *> sinks;
};

// Represents a rectangular feasible region for buffer placement
struct FeasibleRegion
{
    int minX, maxX, minY, maxY;
    FeasibleRegion(int x, int y) : minX(x), maxX(x), minY(y), maxY(y) {}
    FeasibleRegion(int minX_, int maxX_, int minY_, int maxY_)
        : minX(minX_), maxX(maxX_), minY(minY_), maxY(maxY_) {}
};

// A temporary tree structure used during the DME bottom-up pass
struct MergeSubtree
{
    std::vector<Node *> leaves;
    FeasibleRegion region;
    std::vector<MergeSubtree *> children;
    MergeSubtree(Node *leaf) : leaves{leaf}, region(0, 0, 0, 0)
    {
        if (leaf)
        {
            leaves = {leaf};
            region = FeasibleRegion(leaf->x, leaf->y);
        }
    }

    // Constructor for an internal node
    MergeSubtree(const FeasibleRegion &reg, const std::vector<MergeSubtree *> &ch)
        : region(reg), children(ch)
    {
        leaves.clear();
        for (auto *child : ch)
        {
            leaves.insert(leaves.end(), child->leaves.begin(), child->leaves.end());
        }
    }
};