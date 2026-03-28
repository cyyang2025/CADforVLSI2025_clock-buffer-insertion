#pragma once

#include "DataStructures.h"
#include <vector>

// Calculates Manhattan distance between two nodes.
int manhattanDistance(Node *a, Node *b);

// Calculates the path length (arrival time) from the source to a sink.
int calculateArrivalTime(Node *sinkNode);

// Computes the total wire length of the entire tree.
int computeTotalWireLength(Node *root);

// Partitions a set of nodes into two balanced groups based on spatial distribution.
std::pair<std::vector<Node *>, std::vector<Node *>> adaptivePartitionNodes(std::vector<Node *> &nodes);