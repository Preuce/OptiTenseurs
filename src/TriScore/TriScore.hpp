#ifndef TRISCORE_HPP
#define TRISCORE_HPP
#include "../Components.hpp"
#include <vector>

class TriScore{
    public:
    int size;
    Tab G; //une matrice d'adjacence,avec les poids à la fin
    vector<pair<int, double>> R; //La liste des scores triés
    vector<pair<int, int>> E; //la liste des arètes
    Tab V; //la correspondance entre les sommets (tkt)
    vector<pair<int, int>> order;
    //G : matrice d'adjacence, où chaque case G[size*i + j] contient le poids de j par rapport à i (wj si reliés, 0 si soi-même, 1 sinon)
    //pb : mettre à jour G
    //pour tout j tel que G[size*i + j] > 1, on multiplie la ligne G[size*i] avec la ligne G[size*j], G[size*j + i] passe à 1

    Cost solve();
    //met à jour la liste des score après suppression d'une arète
    void updateRatio(); //ou void ?
    //calcule le coût associé à une arète i
    Cost ext_cost(int i, int k);
    void contract(int i, int k);
    int C(int i);

    void display_order();

    void get_size(char* preamble);
    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif