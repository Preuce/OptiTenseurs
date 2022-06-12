#ifndef MATRIXSOLVER_HPP
#define MATRIXSOLVER_HPP
#include "../Components.hpp"

/**
 * @brief algorithme glouton calculant le meilleur coût en testant toutes les paires possibles d'une matrice d'adjacence
 * Complexité (N!)^2
 */

typedef vector<vector<int>> Network; 

Network contract(int i, int j, Network N);
Cost contractionCost(int i, int j, Network N);
void display(Network N);

class MatrixSolver{
    public:
        int size; //nombre de sommets
        Network network; //matrice d'adjacence

        Cost bestCost;
        vector<pair<int, int>> bestOrder;
        std::chrono::duration<double> time;
        void solve(int n, Cost cost, vector<pair<int, int>> v, Network N);
        void display_best_order();
        void init(string file);
        void execfile(string file);
        void execdir(string dir);
};

#endif