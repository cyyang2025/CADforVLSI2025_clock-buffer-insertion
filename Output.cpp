#include "Output.h"
#include "Utils.h"
#include "Globals.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

void generateOutput(const string &filename, Node *src, const vector<Node *> &sinks, const map<string, Node *> &allBuffers)
{
    ofstream outputFile(filename);
    if (!src) return;

    // --- Write Buffers ---
    vector<Node*> sortedBuffers;
    for (auto const& [id, node] : allBuffers) sortedBuffers.push_back(node);
    sort(sortedBuffers.begin(), sortedBuffers.end(), [](Node* a, Node* b) {
        return stoi(a->id.substr(1)) < stoi(b->id.substr(1));
    });

    outputFile << ".buffer " << sortedBuffers.size() << "\n";
    for (Node *buf : sortedBuffers) {
        outputFile << buf->id << " " << buf->x << " " << buf->y << "\n";
    }
    outputFile << ".e\n\n";

    // --- Write Levels ---
    map<Node *, int> nodeLevels;
    queue<Node *> Q;
    nodeLevels[src] = 0;
    Q.push(src);
    int maxLevel = 0;

    while (!Q.empty())
    {
        Node *n = Q.front(); Q.pop();
        int lvl = nodeLevels[n];
        maxLevel = max(maxLevel, lvl);
        for (Node *child : n->children)
        {
            nodeLevels[child] = lvl + 1;
            Q.push(child);
        }
    }

    map<int, vector<Node *>> levels;
    for (auto &nl : nodeLevels) levels[nl.second].push_back(nl.first);

    outputFile << ".level " << maxLevel << endl;
    for (int lvl = 0; lvl < maxLevel; ++lvl)
    {
        outputFile << lvl + 1 << ":";
        bool firstParent = true;
        for (Node *parent : levels[lvl])
        {
            if (!parent->children.empty())
            {
                if (!firstParent) outputFile << " ";
                outputFile << parent->id << "{";
                for (size_t i = 0; i < parent->children.size(); ++i)
                {
                    outputFile << parent->children[i]->id;
                    if (i != parent->children.size() - 1) outputFile << " ";
                }
                outputFile << "}";
                firstParent = false;
            }
        }
        outputFile << endl;
    }
    outputFile << ".e\n\n";

    // --- Write Stats ---
    int T_max = numeric_limits<int>::min();
    int T_min = numeric_limits<int>::max();
    for (Node *sink : sinks)
    {
        int arrTime = calculateArrivalTime(sink);
        T_max = max(T_max, arrTime);
        T_min = min(T_min, arrTime);
    }
    int W_cbi = computeTotalWireLength(src);
    outputFile << "T_max: " << T_max << ", T_min: " << T_min << ", W_cbi: " << W_cbi << endl;
    
    // Also print to console for convenience
    cout << "\n--- Results ---" << endl;
    cout << "T_max: " << T_max << ", T_min: " << T_min << ", W_cbi: " << W_cbi << endl;
    cout << "Buffer Count: " << allBuffers.size() << endl;
}