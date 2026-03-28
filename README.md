# Clock Buffer Insertion (CBI) Program

## Overview
This program implements a clock buffer insertion algorithm for Clock Tree Synthesis (CTS) as part of CAD Design Project 2. The program reads a CBI input file containing clock source and sink coordinates along with design constraints, then generates an optimized clock tree with buffer insertion to minimize clock skew and wire length.

## Project Structure
The program is organized into multiple source files as defined in the Makefile:

- **M11415069.cpp** - Main program entry point and core algorithms
- **Parser.cpp** - Input file parsing functionality
- **Utils.cpp** - Utility functions and helper methods
- **Overlap.cpp** - Component overlap detection and handling
- **DME.cpp** - Deferred Merge Embedding algorithms
- **Output.cpp** - Output file generation
- **Globals.cpp** - Global variables and constants

## Compilation Requirements
- **Compiler**: g++ with C++17 support
- **Operating System**: Linux environment (tested on WSL)
- **Build System**: Make

## Compilation Instructions
To compile the program, simply run:
```bash
make
```

This will generate the executable `cbi` in the current directory.

To clean build artifacts:
```bash
make clean
```

## Usage
The program takes two command-line arguments:
```bash
./cbi INPUT_FILE OUTPUT_FILE
```

**Parameters:**
- `INPUT_FILE` - Path to the input CBI file containing constraints and pin coordinates
- `OUTPUT_FILE` - Path where the output CBI file will be written

## Input File Format
The input file follows this structure:

```
.limit
fanout <max_fanout>        # Maximum number of fanouts per node
length <max_length>        # Maximum total wire length constraint
.dimx <chip_width>         # Chip dimension in x-axis
.dimy <chip_height>        # Chip dimension in y-axis
.e

.pin <num_pins>
<x1> <y1>                  # Clock source (SRC) coordinates
<x2> <y2>                  # Clock sink S1 coordinates
<x3> <y3>                  # Clock sink S2 coordinates
...
.e
```

## Output File Format
The program generates an output file containing:

1. **Buffer Section**: Lists all inserted clock buffers with coordinates
```
.buffer <num_buffers>
B1 <x> <y>
B2 <x> <y>
...
.e
```

2. **Level Section**: Describes the hierarchical clock tree structure
```
.level <num_levels>
1:SRC{B1 B2}
2:B1{S1 S3} B2{S2 S4}
...
.e
```

3. **Performance Metrics**: 
```
T_max: <max_arrival_time>, T_min: <min_arrival_time>, W_cbi: <total_wire_length>
```

## Algorithm Features
- **Adaptive Partitioning**: Uses 2D adaptive partitioning that splits nodes based on the dimension with greatest spread
- **Buffer Placement**: Employs weighted median placement for optimal buffer positioning
- **Position Refinement**: Iterative local optimization to minimize total wire length
- **Constraint Handling**: Respects fanout and wire length constraints specified in input
- **Skew Minimization**: Optimizes clock tree to reduce arrival time differences

## Example Usage
```bash
# Compile the program
make

# Run with sample input
./cbi input.cbi output.cbi

# View the generated output
cat output.cbi
```

## Performance Metrics Calculated
- **T_max**: Maximum arrival time among all clock sinks
- **T_min**: Minimum arrival time among all clock sinks  
- **W_cbi**: Total wire length of the clock tree with buffers

## Technical Details
- Uses Manhattan distance calculation for wire length estimation
- Implements breadth-first traversal for level assignment
- Ensures no component overlapping in final placement
- Supports arbitrary chip dimensions and pin counts
- Memory management with proper cleanup of dynamic allocations

## Dependencies
- Standard C++ libraries (iostream, vector, map, queue, algorithm, etc.)
- No external dependencies required

## Notes
- All coordinates are integers within the specified chip dimensions (0,0) to (dimX, dimY)
- Clock source (SRC) is always at level 0
- Each component appears exactly once in the tree hierarchy
- Buffer IDs are automatically generated as B1, B2, B3, etc.
- Sink IDs are generated as S1, S2, S3, etc. based on input order