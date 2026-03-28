#include "Globals.h"

// Definitions of global variables
int maxFanout = 0;
int maxWireLength = 0;
int dimX = 0, dimY = 0;
int bufferCounter = 0;

// Optimization flags
bool refineForTotalWireLength = false;
int tryMove = 2;
int iterations = 10;