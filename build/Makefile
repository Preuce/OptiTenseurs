SolverMatrix: ../src/Matrice/MatrixSolver.cpp ../src/Matrice/MatrixSolver.hpp ../src/Matrice/Main.cpp
	g++ ../src/Matrice/MatrixSolver.cpp ../src/Matrice/MatrixSolver.hpp ../src/Matrice/Main.cpp -o SolverMatrix

SolverClasses: ../src/Classes/Classes.cpp ../src/Classes/ClassesSolver.cpp
	g++ ../src/Classes/Classes.cpp ../src/Classes/ClassesSolver.cpp -o SolverClasses

SolverSplit: ../src/Split/SplitSolver.cpp ../src/Split/SplitSolver.hpp ../src/Split/Main.cpp
	g++ ../src/Split/SplitSolver.cpp ../src/Split/SplitSolver.hpp ../src/Split/Main.cpp -o SolverSplit

SolverVSplit: ../src/RangeSplit/RangeSplitSolver.cpp ../src/RangeSplit/RangeSplitSolver.hpp ../src/Split/SplitSolver.cpp ../src/RangeSplit/Main.cpp
	g++ ../src/RangeSplit/RangeSplitSolver.cpp ../src/RangeSplit/RangeSplitSolver.hpp ../src/Split/SplitSolver.cpp ../src/RangeSplit/Main.cpp -o SolverVSplit

SolverSide: ../src/Side/SideExSolver.cpp ../src/Side/SideExSolver.hpp ../src/Side/Main.cpp
	g++ ../src/Side/SideExSolver.cpp ../src/Side/SideExSolver.hpp ../src/Side/Main.cpp -o SolverSide

SolverTriScore: ../src/TriScore/TriScore.cpp ../src/TriScore/TriScore.hpp ../src/TriScore/Main.cpp
	g++ ../src/TriScore/TriScore.cpp ../src/TriScore/TriScore.hpp ../src/TriScore/Main.cpp -o SolverTriScore
	
Solver: ../src/TriScore/TriScore.cpp ../src/Side/SideExSolver.cpp ../src/RangeSplit/RangeSplitSolver.cpp ../src/Split/SplitSolver.cpp ../src/Matrice/MatrixSolver.cpp ../src/Main.cpp
	g++ ../src/TriScore/TriScore.cpp ../src/Side/SideExSolver.cpp ../src/RangeSplit/RangeSplitSolver.cpp ../src/Split/SplitSolver.cpp ../src/Matrice/MatrixSolver.cpp ../src/Main.cpp -o Solver

InitInstances: ../src/InstancesGenerator/InstanceGenerator.cpp ../src/InstancesGenerator/InstanceGenerator.hpp
	g++ ../src/InstancesGenerator/InstanceGenerator.cpp ../src/InstancesGenerator/InstanceGenerator.hpp -o InitInstances

clean:
	rm -f *.o
