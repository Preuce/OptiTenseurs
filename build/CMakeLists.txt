cmake_minimum_required(VERSION 3.16)

project(OptiTenseurs)

set(TOOLS
    ../tools/Display/Display.cpp
    ../tools/Sort/Sort.cpp
    )

#SolverMatrix
set(MATRIX ../src/Matrice/)

#SolverTriScore
set(TRISCORE ../src/TriScore/)

#SolverSideR
#set(SIDEREC ../src/SideRec/)

#SolverSideI
set(SIDEITER ../src/SideIter/)

#SolverSplit
set(SPLIT ../src/Split/)

#SolverVSplit
set(VSPLIT ../src/RangeSplit/)

#SolverSSpit
set(SPLITSMART ../src/SplitSmart/)

#SolverSimpleG
set(SIMPLEGREEDY ../src/SimpleGreedy/)

#InitInstances
set(INITINSTANCES ../tools/InstancesGenerator/)

#Compute
set(COMPUTE ../tools/Compute/)


set(ALGORITHMS
    ${TRISCORE}TriScore.cpp 
    #${SIDEREC}SideExSolver.cpp 
    ${VSPLIT}RangeSplitSolver.cpp 
    ${SPLIT}SplitSolver.cpp 
    ${MATRIX}MatrixSolver.cpp 
    ${SIDEITER}SideExSolver.cpp 
    ${SIMPLEGREEDY}SimpleG.cpp
    )

add_executable(SolverMatrix ${MATRIX}MatrixSolver.cpp ${MATRIX}Main.cpp ${TOOLS})
add_executable(SolverSimpleG ${SIMPLEGREEDY}SimpleG.cpp ${SIMPLEGREEDY}Main.cpp ${TOOLS})
add_executable(SolverSplit ${SPLIT}SplitSolver.cpp ${SPLIT}Main.cpp ${TOOLS})
add_executable(SolverVSplit ${VSPLIT}RangeSplitSolver.cpp ${VSPLIT}Main.cpp ${SPLIT}SplitSolver.cpp ${TOOLS})
add_executable(SolverSSplit ${SPLITSMART}SplitSmart.cpp ${SPLITSMART}Main.cpp ${TOOLS})
add_executable(SolverSideI ${SIDEITER}SideExSolver.cpp ${SIDEITER}Main.cpp ${TOOLS})
add_executable(SolverTriScore ${TRISCORE}TriScore.cpp ${TRISCORE}Main.cpp ${TOOLS})
add_executable(Solver ../src/Main.cpp ${ALGORITHMS} ${TOOLS})
add_executable(InitInstances ${INITINSTANCES}InstanceGenerator.cpp)
add_executable(Compute ${COMPUTE}Test.cpp ${TOOLS})
