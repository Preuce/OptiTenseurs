#ifndef EdgeSplit_HPP
#define EdgeSplit_HPP
#include "../Components.hpp"
#include "../TriScore/TriScore.hpp"

void affiche(int i1, int i2, int j1, int j2);
void affiche_A(Tab A);

class ESplit : public Algorithm{
    public:
    vector<pair<int, int>> E;
    CostTab A; //(size*(size+1)) stock le poids sortant des sommets en fonction de la taille du système, ainsi que le poids du système
    CostTab W; //(3*size/2 - 2) stock le poids des arêtes
    unordered_map<unsigned long long, Cost> M;

    Cost solve(int i1, int i2, int j1, int j2, CostTab A);
    Cost solve_vertical(int i1, int i2, int j1, int j2, unsigned long long key, CostTab A, Cost S_cost);
    Cost solve_diag(int i1, int i2, int j1, int j2, unsigned long long key, CostTab A, Cost S_cost);

    void ext_cost(int i1, int i2, int j1, int j2, CostTab A, Cost& s);

    unsigned long long convert(int i1, int i2, int j1, int j2);

    int D;

    void display_order();
    
    void init(string file);
    Cost call_solve();
};

#endif