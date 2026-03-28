#pragma once

#include "DataStructures.h"

// Generates the final output file based on the constructed clock tree.
void generateOutput(const std::string &filename, Node *src, const std::vector<Node *> &sinks, const std::map<std::string, Node *> &allBuffers);