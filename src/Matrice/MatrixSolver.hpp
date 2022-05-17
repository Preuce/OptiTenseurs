#ifndef MATRIXSOLVER_HPP
#define MATRIXSOLVER_HPP
#include "ReseauMatrix.hpp"
#include <vector>
#include <string.h>
using namespace std;

class MatrixSolver{
    public:
        unsigned int bestCost;
        vector<pair<int, int>> bestOrder;

        MatrixNetwork RefMatrix;

        void all_perm(int n, int cost, vector<pair<int, int>> v, Network N);
        
        void display_best_order();
        void solveSansMemo();
        void init(const char* path);
        void execdir(const char* dir);
        void execfile(const char* file);
};

MatrixSolver solver;

#endif