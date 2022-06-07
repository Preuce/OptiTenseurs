#ifndef MATRIXSOLVER_HPP
#define MATRIXSOLVER_HPP
#include "../Components.hpp"

typedef vector<vector<int>> Network;

Network contract(int i, int j, Network N);
Cost contractionCost(int i, int j, Network N);
void display(Network N);

class MatrixSolver{
    public:
        int size;
        Network network;

        Cost bestCost;
        vector<pair<int, int>> bestOrder;
        std::chrono::duration<double> time;
        void solve(int n, Cost cost, vector<pair<int, int>> v, Network N);

        void display();
        void get_size(char* Preamble);
        
        void display_best_order();
        void init(const char* file);
        void execfile(const char* file);
        void execdir(const char* dir);
};

#endif