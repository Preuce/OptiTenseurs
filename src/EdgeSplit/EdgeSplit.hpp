#ifndef EdgeSplit_HPP
#define EdgeSplit_HPP
#include "../Components.hpp"
#include "../TriScore/TriScore.hpp"
#include <unordered_map>

class ESplit{
    public:
    int size;
    int delta = -1;
    vector<pair<int, int>> E;
    Tab G; //(size*(size+1)) matrice d'adjacence des sommets + poids sortant stockés sur la dernière colonne
    Tab A; //(size*(size+1)) stock le poids sortant des sommets en fonction de la taille du système, ainsi que le poids du système
    Tab W; //(3*size/2 - 2) stock le poids des arêtes
    unordered_map<unsigned long long, int> M;

    int single = 0;
    int multiple = 0;

    Cost bestCost;
    Tab bestOrder;
    std::chrono::duration<double> time;
    

    Cost solve(int i1, int i2, int j1, int j2, Tab A);
    Cost solve_vertical(int i1, int i2, int j1, int j2, unsigned long long key, Tab A);
    Cost solve_diag(int i1, int i2, int j1, int j2, unsigned long long key, Tab A);
    Cost solve_U(int i1, int i2, int j1, int j2, int start_z_cost, int end_x_cost, int ofsy, int ofs);
    Cost solve_D(int i1, int i2, int j1, int j2, int start_x_cost, int end_z_cost, int ofsy, int ofs);

    Cost ext_cost(int i1, int i2, int j1, int j2);

    unsigned long long convert(int i1, int i2, int j1, int j2);

    int D;

    void display_order();
    
    void init(string file);
    void execfile(string file);
    void execdir(string dir);
};

#endif