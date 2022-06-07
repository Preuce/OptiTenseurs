#ifndef SIDEEXSOLVER_HPP
#define SIDEEXSOLVER_HPP
#include "../Components.hpp"

class SLim{
    public:
    public:
    int size;
    int delta;
    //(on rappelle que D = size/2)
    Tab A; //poids sortant (taille size);
    Tab G; //matrice d'adjacence (taille size*(size+1))
    Tab T; //tableau stockant les ti (taille 2(D-1))

    Tab P; //size
    Tab C; //tableau stockant les coûts ci (taille 2(D-1)), C[i] = cout pour obtenir T[i]
    
    Tab Z; //size/2, donne la référence de R et S
    vector<pair<int, int>> O; //tableau stockant l'ensemble des pairs de contractions donnant des coûts minimum

    Tab bestOrder;
    int bestCost;
    std::chrono::duration<double> time;

    Cost solve();

    Cost contract(int s, int i, int x, pair<int, int>& p);
 
    void computeA(int s, int k);
    void restoreA(int s);
    void display_order(int s, int k);
    void display_order();
    void get_order(int s, int k);

    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif