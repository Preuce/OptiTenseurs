# OptiTenseurs
The goal of this project is to implement algorithms capable of finding the best order of contraction of any tensor train, given the size of each node's dimension.

# Structure
* the 'src' directory contains all the main algorithms (divided in their own sub-directories) as well as a main .cpp that will use them and store the results
* the 'calc' directory contains text files that can be read to test contractions order for specific instances
* the 'tools' directory contains quality-of-life programs such as the code that displays a TT, the code to test contraction orders, and the code to generate instances
* the 'build' directory contains a CMakeLists that can compile everything
* the 'result' directory contains all results of of the main Solver's execution, including 2 csv files and a general r
* the 'plot' directory contains a python notebook with a piece of code to plot any cost or time csv

# Compiling
When in the 'build' directory, use 'cmake .' to set up the compiler, then 'make' followed by the name of the program (or none to compile everything). Here are the programs you can build :
* Compute, to test the cost of a specific contraction order
* InitInstances, to generate instances
* Solver, main program that can execute every other solver, display the TT, and export the results
* SolverMatrix, algorithm iterating on every pair of tensors (O(n!^2))
* SolverTS, heuristic that sorts every edges by weights^2/contraction_cost, and contracts the edge with the best ratio at each step
* SolverMTS, uses TS's order as reference, and shakes it to try and improve it using a user-defined parameter DELTA (not fonctioning)
* SolverNTS, same as TS but defines the order to follow at the beggining and never updates it
* SolverSide, heuristic that contracts a TT from a side (from left to right, dimension by dimension), but limits how many central edges can merge in a row, using a user-defined parameter DELTA.
* SolverSplit, algorithm iterating on every split of 2 sets of tensors
* SolverVSplit, heuristic that splits the TT in pieces of max size DELTA, then solves them using any exact algorithm
* SolverSimpleG, algorithm that tests every existing contraction order, by only considering edges
* SolverEdgeSplit, heuristic that splits the TT in (exactly) 2 convex shapes

# Instances
Instances are text files located in the 'instances' directory. They can be read by the programs in order to be solved.
They are formatted in the following way :
* Lines starting with 'c' will be displayed during the solving of the TT, but do not affect it in any way
* Lines starting with 'v' are a representation of the TT
* One single line starts with 'p' followed by the size (in number of node) of the TT
* Lines starting with 'e' describe edges, they read 'node1 node2 weight'

# Generating instances
Instances can be generated using ./InitInstances while in the 'build' directory.  
The parameters are the following :
* 'd path' defines the path ('instances/' is considered as the root directory, default : test/instance)
* 'n number' the amount of instances to generate (default : 1)
* 's number' the size of the TT to generate (default : 6)
* 'm number' the minimum weight present in the TT (default : 1)
* 'M number' the maximum weight present in the TT (default : 9)

# Testing
You can test contractions orders on specific instances by using the Compute program  
Compute takes a text file from the 'calc' directory (test.txt by default) and reads it.  
These text file must be formatted in the following way :
* 'f instance_file'
* 'o number_edge1 number_edge2 number_edge3...'
There is no limit to how many file/order you can put

# Commands
Use './Program instance_file' or './Program d directory' to use an algorithm on a single TT, or on an entire directory. ('instances/' is considered as the root directory)  
On top of this, you can use './Solver v instance_file' or './Solver vd directory' to display a single instance, or an entire directory.  
If a heuristic uses an extra parameter DELTA, it can be set using 'r value'.  
You can define a timeout value in Main.hpp after which an algorithm will stop its execution, and will never be called again.  
Instances are sorted by name, make sure they each contain a number, and are sorted from lowest size to highest

# Results
The result of the execution of ./Solver can be found in the 'results' directory, 'results.txt' contains a general recap of the execution. Can be seen : the time, best cost and (if available) the best order for each algorithm.
'cost.csv' contains every costs, 'time.csv' contains every time.  

# Notations
* size : the size of the instance
* G is an adjacence matrix, G[i*size+j] gives the weight of the edge between the tensors i and j. G is usually of size (size*size) but can sometime have an extra column indicating the total weight of each tensors at an individual scale
* A is a list (sometime a list of lists) indicating the total weight of each tensors, considering that they may already be part of a greater tensor-network
* C stores the cost of every order, its type may change depending on the way the contraction orders are encoded, but it usually is either a list or an unordered_map
* S usually indicates a list of tensor, or a list of edge, but it almost always is the list we are iterating on
* P or O usually stores contraction orders, though getting it requires to use a dedicated fonction
* The 'Cost' type is int, but it could be changed to avoid overflows
* The 'CostTab' type is vector<Cost>
* The 'Tab' type is vector<int>
