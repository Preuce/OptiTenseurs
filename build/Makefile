SolverMatrix: ../src/Matrice/ReseauMatrix.cpp ../src/Matrice/MatrixSolver.cpp
	g++ ../src/Matrice/ReseauMatrix.cpp ../src/Matrice/MatrixSolver.cpp -o SolverMatrix

SolverClasses: ../src/Classes/Classes.cpp ../src/Classes/ClassesSolver.cpp
	g++ ../src/Classes/Classes.cpp ../src/Classes/ClassesSolver.cpp -o SolverClasses

SolverSplit: ../src/Split/SplitSolver.cpp ../src/Split/SplitSolver.hpp ../src/Split/Main.cpp
	g++ ../src/Split/SplitSolver.cpp ../src/Split/SplitSolver.hpp ../src/Split/Main.cpp -o SolverSplit

SolverVSplit: ../src/RangeSplit/RangeSplitSolver.cpp ../src/RangeSplit/RangeSplitSolver.hpp ../src/Split/SplitSolver.cpp ../src/RangeSplit/Main.cpp
	g++ ../src/RangeSplit/RangeSplitSolver.cpp ../src/RangeSplit/RangeSplitSolver.hpp ../src/Split/SplitSolver.cpp ../src/RangeSplit/Main.cpp -o SolverVSplit

SolverTriScore: ../src/TriScore/Triscore.cpp
	g++ ../src/TriScore/Triscore.cpp -o SolverTriScore

InitInstances: ../src/InstancesGenerator/InstanceGenerator.cpp
	g++ ../src/InstancesGenerator/InstanceGenerator.cpp -o InitInstances
