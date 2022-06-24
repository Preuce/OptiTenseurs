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

class MatrixSolver : public Algorithm{
    public:
        Network network; //matrice d'adjacence

        void solve(int n, Cost cost, vector<pair<int, int>> v, Network N);
        void display_order();
        
        void init(string file);
        Cost call_solve();
};

#endif