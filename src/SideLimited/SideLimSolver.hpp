#ifndef SIDEEXSOLVER_HPP
#define SIDEEXSOLVER_HPP
#include "../Components.hpp"

class SLim{
    public:
    int size;
    Tab A; //les poids sortants pour un état s donné
    Tab G;

    Tab R; //Cs[k] + contract(0, s)
    Tab Q; //Cs[k] + contract(1, s)

    //les couts, de dimension (size*size)/4
    vector<Cost> C;
    //le poids de l'arète t de l'état s
    vector<Cost> T;

    //taille 2D a priori, pour chaque état on va stocker "l'indice" de l'état précédent 
    //(0 et 1 c'est les cas basiques, ce qui est embêtant c'est les 2 -> 2i - 1, où i est size - S.size()-1)
    //Pour tout i correspondant à size-S.size()-1
    //size * (2*size-1)/2
    //(i+1) * (2i)/2
    //dimension (2*size-1)*size
    vector<int> P;
    //on stock dans chaque P[size*s + i] l'indice p vers lequel on va
    vector<pair<int, int>> Z; //paires de contraction temporaires (taille s)
    vector<pair<int, int>> O; //ensemble des pairesde contractions (taille s*s/4)

    Cost solve(int s, int i);

    //computer A en considérant que les 2 contract° ont été effectuée
    void computeA(int s);
    void restoreA(int s);
    Cost contract(int x, int s);

    void display_order(int s, int i);

    void get_size(char* preamble);
    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif