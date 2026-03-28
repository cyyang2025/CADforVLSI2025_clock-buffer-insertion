#pragma once

#include "DataStructures.h"

// Builds the abstract DME tree recursively from the bottom up.
MergeSubtree* bottomUpDME(std::vector<Node*> sinks);

// Embeds the abstract tree into the physical layout from the top down, placing buffers.
void topDownEmbed(Node* parent, MergeSubtree* subtree, std::map<std::string, Node *> &allBuffers, Node* root, const Node *src, const std::vector<Node*> &sinks);