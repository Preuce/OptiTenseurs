#ifndef TRISCORE_HPP
#define TRISCORE_HPP
#include "../Components.hpp"

class TriScore{
    public:
    int size;
    Tab G; //une matrice d'adjacence
    vector<pair<int, double>> R; //La liste des scores triés
    vector<pair<int, int>> E; //la liste des arètes
    Tab V; //la correspondance entre les sommets (tkt)

    Tab bestOrder; //l'ordre
    int bestCost;
    std::chrono::duration<double> time;

    Cost solve();
    //met à jour la liste des score après suppression d'une arète
    void updateRatio();
    //calcule le coût associé à une arète (i, k)
    Cost ext_cost(int i, int k);
    void contract(int i, int k);
    int C(int i);

    void display_order();

    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif