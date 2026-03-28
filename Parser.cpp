#include "Parser.h"
#include "Globals.h" // Access to global variables like dimX, dimY, etc.
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

ParsedData parseInput(const string &filename)
{
    ParsedData data;
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return data;
    }
    string line;
    bool in_limit_section = false, in_pin_section = false;
    int num_pins = 0, pins_read = 0;

    while (getline(inputFile, line))
    {
        if (line.empty() || line.find_first_not_of(" \t\n\r\f\v") == string::npos)
            continue;
        stringstream ss(line);
        string keyword;
        ss >> keyword;

        if (keyword == ".limit")
        {
            in_limit_section = true;
            in_pin_section = false;
            continue;
        }
        if (keyword == ".pin")
        {
            in_pin_section = true;
            in_limit_section = false;
            ss >> num_pins;
            continue;
        }
        if (keyword == ".e")
        {
            in_limit_section = false;
            in_pin_section = false;
            continue;
        }

        if (in_limit_section)
        {
            if (keyword == "fanout")
                ss >> maxFanout;
            else if (keyword == "length")
                ss >> maxWireLength;
            if (keyword == ".dimx")
                ss >> dimX;
            else if (keyword == ".dimy")
                ss >> dimY;
        }
        else if (in_pin_section)
        {
            if (pins_read < num_pins)
            {
                ss.clear();
                ss.str(line);
                int x, y;
                ss >> x >> y;
                if (data.src == nullptr)
                {
                    data.src = new Node("SRC", x, y);
                }
                else
                {
                    data.sinks.push_back(new Node("S" + to_string(pins_read), x, y));
                }
                pins_read++;
            }
        }
    }
    return data;
}