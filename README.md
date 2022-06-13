# OptiTenseurs
The goal of this project is to implement algorithms capable of finding the best order of contraction of any tensor train, given the size of each node's dimension.

# Structure
* the 'src' directory contains all the main algorithms (divided in their own sub-directories) as well as a main .cpp that will use them and store the results
* the 'calc' directory contains text files that can be read to test contractions order for specific instances
* the 'tools' directory contains quality-of-life programs such as the code that displays a TT, the code to test contraction orders, and the code to generate instances
* the 'build' directory contains a CMakeLists that can compile everything

# Compiling
When in the 'build' directory, use 'cmake .' to set up the compiler, then 'make' followed by the name of the program (or none to compile everything). Here are the programs you can build :
* Compute, to test the cost of a specific contraction order
* InitInstances, to generate instances
* Solver
* SolverMatrix
* SolverTS
* SolverMTS
* SolverNTS
* SolverSideI
* SolverSideL
* SolverSplit
* SolverVSplit
* SolverSimpleG

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
* 'd directory' to choose where to create the files ('instances/' is considered as the root directory)
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

# Results
The result of the execution of ./Solver can be found in 'results/results.txt'
Can be seen the time, best cost and (if available) the best order for each algorithm.