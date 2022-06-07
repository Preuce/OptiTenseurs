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

#SolverMTriScore
set(TRISCOREM ../src/TriScoreMargin)

#SolverSideI
set(SIDEITER ../src/SideIter/)

#SolverSideL
set(SIDERELIM ../src/SideLimited/)

#SolverSplit
set(SPLIT ../src/Split/)

#SolverVSplit
set(VSPLIT ../src/VerticalSplit/)

#SolverSimpleG
set(SIMPLEGREEDY ../src/SimpleGreedy/)

#InitInstances
set(INITINSTANCES ../tools/InstancesGenerator/)

#Compute
set(COMPUTE ../tools/Compute/)


set(ALGORITHMS
    ${MATRIX}MatrixSolver.cpp 
    ${TRISCORE}TriScore.cpp 
    #${TRISCOREM}TriScoreMargin.cpp
    ${VSPLIT}VerticalSplitSolver.cpp 
    ${SPLIT}SplitSolver.cpp 
    ${SIDEITER}SideExSolver.cpp
    #${SIDELIM}SideLimSolver.cpp
    ${SIMPLEGREEDY}SimpleG.cpp
    )

add_executable(SolverMatrix ${MATRIX}MatrixSolver.cpp ${MATRIX}Main.cpp ${TOOLS})
add_executable(SolverSimpleG ${SIMPLEGREEDY}SimpleG.cpp ${SIMPLEGREEDY}Main.cpp ${TOOLS})
add_executable(SolverSplit ${SPLIT}SplitSolver.cpp ${SPLIT}Main.cpp ${TOOLS})
add_executable(SolverVSplit ${VSPLIT}VerticalSplitSolver.cpp ${VSPLIT}Main.cpp ${SPLIT}SplitSolver.cpp ${TOOLS})
add_executable(SolverSideI ${SIDEITER}SideExSolver.cpp ${SIDEITER}Main.cpp ${TOOLS})
#add_executable(SolverSideL ${SIDELIM}SideLimSolver.cpp ${SIDELIM}Main.cpp ${TOOLS})
add_executable(SolverTriScore ${TRISCORE}TriScore.cpp ${TRISCORE}Main.cpp ${TOOLS})
#add_executable(SolverMTriScore ${TRISCOREM}TriScoreMargin.cpp ${TRISCOREM}Main.cpp ${TOOLS})
add_executable(Solver ../src/Main.cpp ${ALGORITHMS} ${TOOLS})
add_executable(InitInstances ${INITINSTANCES}InstanceGenerator.cpp)
add_executable(Compute ${COMPUTE}Test.cpp ${TOOLS})
