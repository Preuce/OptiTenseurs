cmake_minimum_required(VERSION 3.16)

project(OptiTenseurs)

set(TOOLS
    ../tools/Display/Display.cpp
    ../tools/Sort/Sort.cpp
    )

#SolverMatrix
set(MATRIX ../src/Matrice/)

#SolverTriScore
set(TS ../src/TriScore/)

#SolverMTS
set(MTS ../src/TriScoreMargin/)

#SolverNTS
set(NTS ../src/TriScoreNaive/)

#SolverSideI
set(SIDEITER ../src/SideIter/)

#SolverSideL
set(SIDELIM ../src/SideLimited/)

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
    ${TS}TriScore.cpp
    ${MTS}TriScoreM.cpp
    ${NTS}TriScoreN.cpp
    ${VSPLIT}VerticalSplitSolver.cpp 
    ${SPLIT}SplitSolver.cpp 
    ${SIDEITER}SideExSolver.cpp
    ${SIDELIM}SideLimSolver.cpp
    ${SIMPLEGREEDY}SimpleG.cpp
    )

add_executable(SolverMatrix ${MATRIX}MatrixSolver.cpp ${MATRIX}Main.cpp ${TOOLS})
add_executable(SolverSimpleG ${SIMPLEGREEDY}SimpleG.cpp ${SIMPLEGREEDY}Main.cpp ${TOOLS})
add_executable(SolverSplit ${SPLIT}SplitSolver.cpp ${SPLIT}Main.cpp ${TOOLS})
add_executable(SolverVSplit ${VSPLIT}VerticalSplitSolver.cpp ${VSPLIT}Main.cpp ${SPLIT}SplitSolver.cpp ${TOOLS})
add_executable(SolverSideI ${SIDEITER}SideExSolver.cpp ${SIDEITER}Main.cpp ${TOOLS})
add_executable(SolverSideL ${SIDELIM}SideLimSolver.cpp ${SIDELIM}Main.cpp ${TOOLS})
add_executable(SolverTriScore ${TS}TriScore.cpp ${TS}Main.cpp ${TOOLS})
add_executable(SolverMTS ${MTS}TriScoreM.cpp ${MTS}Main.cpp ${TS}TriScore.cpp ${TOOLS})
add_executable(SolverNTS ${NTS}TriScoreN.cpp ${NTS}Main.cpp ${TOOLS})
add_executable(Solver ../src/Main.cpp ${ALGORITHMS} ${TOOLS})
add_executable(InitInstances ${INITINSTANCES}InstanceGenerator.cpp)
add_executable(Compute ${COMPUTE}Test.cpp ${TOOLS})
