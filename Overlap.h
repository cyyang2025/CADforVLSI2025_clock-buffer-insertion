#pragma once

#include "DataStructures.h"

// Checks if a given (x, y) coordinate is already occupied.
bool isOccupied(int x, int y, const std::map<std::string, Node*> &allBuffers, const Node *src, const std::vector<Node*> &sinks);

// Finds a nearby non-occupied position if the original is taken.
std::pair<int, int> findNonOverlappingPosition(int originalX, int originalY, const std::map<std::string, Node*> &allBuffers, const Node *src, const std::vector<Node*> &sinks, int maxJitter = 3);

// Performs a final check for any overlaps in the entire design.
bool checkOverlapping(const std::map<std::string, Node*> &allBuffers, const Node *src, const std::vector<Node*> &sinks);