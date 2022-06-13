#ifndef SPLITSMART_HPP
#define SPLITSMART_HPP
#include "../Components.hpp"
#include "../TriScore/TriScore.hpp"

class SS{
    public:
    int size;
    int delta = -1;
    vector<pair<int, int>> E;
    Tab G; //(size*(size+1)) matrice d'adjacence des sommets + poids sortant stockés sur la dernière colonne
    Tab A; //(size*(size+1)) stock le poids sortant des sommets en fonction de la taille du système, ainsi que le poids du système
    Tab C; //(size/2 + 1)^4, le coût associé à chaque quadruplet (i1, i2, j1, j2)

    Cost bestCost;
    Tab bestOrder;
    std::chrono::duration<double> time;

    enum shape{ //pas forcément nécessaire d'avoir les orientations et autre
        Z, //Z, bloc avec 2 bouts opposés en plus
        BLOC, //2 lignes alignées
        LINE, //ligne
        T, //T, bloc avec 2 bouts sur la même ligne en plus
        CRUNCH, //bloc avec bout en plus
        MONO, //singleton, sommet seul, inséparable
        BUCKET, //god someone help this lad, fait passer en 2*N^4
    };

    Cost solve(int i1, int i2, int j1, int j2);
    Cost cut();
    Cost ext_cost(Tab S);
    void computeA(Tab S1, Tab S2);

    int D3;
    int D2;
    int D;
    inline int* C_at(int i1, int i2, int j1, int j2){
        return &(C[D3*i1 + D2*i2 + D*j1 + j2]);
    }

    shape match_shape(int i1, int i2, int j1, int j2); //c'est un peu moche tout ça
    Cost solve_Z(int i1, int i2, int j1, int j2);
    Cost solve_Bloc(int i1, int i2, int j1, int j2);
    Cost solve_Line(int i1, int i2, int j1, int j2);
    Cost solve_T(int i1, int i2, int j1, int j2);
    Cost solve_crunch(int i1, int i2, int j1, int j2);
    Cost solve_Bucket(int i1, int i2, int j1, int j2);

    void display_order();
    
    void init(string file);
    void execfile(string file);
    void execdir(string dir);
};

#endif