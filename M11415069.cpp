#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "DataStructures.h"
#include "Parser.h"
#include "DME.h"
#include "Overlap.h"
#include "Output.h"

// Helper function to clean up all dynamically allocated memory
void cleanup(ParsedData& data, std::map<std::string, Node*>& buffers) {
    for (auto& pair : buffers) {
        delete pair.second;
    }
    for (Node* sink : data.sinks) {
        delete sink;
    }
    if (data.src) {
        delete data.src;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::cout << "Parsing input file: " << argv[1] << std::endl;
    ParsedData data = parseInput(argv[1]);
    if (data.src == nullptr)
    {
        std::cerr << "Error: Clock source (SRC) not found or parsed correctly." << std::endl;
        return 1;
    }
    
    // Check initial overlapping between source and sinks
    if (checkOverlapping({}, data.src, data.sinks))
    {
        std::cerr << "Error: Initial overlapping detected between source and sinks." << std::endl;
        cleanup(data, *(new std::map<std::string, Node*>())); // Pass empty map for buffers
        return 1;
    }

    std::map<std::string, Node *> allBuffers;
    std::cout << "Running DME algorithm..." << std::endl;
    MergeSubtree* dmeTree = bottomUpDME(data.sinks);
    topDownEmbed(data.src, dmeTree, allBuffers, data.src, data.src, data.sinks);

    // Final overlapping check
    if (checkOverlapping(allBuffers, data.src, data.sinks))
    {
        std::cerr << "Error: Component overlapping detected in the final design." << std::endl;
        cleanup(data, allBuffers);
        return 1;
    }
    
    std::cout << "Generating output file: " << argv[2] << std::endl;
    generateOutput(argv[2], data.src, data.sinks, allBuffers);

    std::cout << "\nClock tree synthesis complete. Output written to " << argv[2] << std::endl;

    cleanup(data, allBuffers);

    return 0;
}