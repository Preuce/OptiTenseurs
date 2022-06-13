#ifndef TRISCOREMARGIN_HPP
#define TRISCOREMARGIN_HPP
#include "../Components.hpp"
#include "../TriScore/TriScore.hpp"

/**
 * @brief Version alternative de TriScore, 
 * modifie légèrement l'ordre déterminé par TriScore afin de trouver un meilleur résultat
 */

class SousG{
    public:
    Tab S;
    Tab G;
    Tab V;

    int C(int i);
    void set(Tab G, Tab V){this->G = G; this->V = V;};
};

class TriScoreM{
    public:
    TriScore triscore;
    int size;
    int delta;
    vector<pair<int, int>> E;
    Tab G;

    Tab bestOrder;
    Cost bestCost;
    std::chrono::duration<double> time;

    SousG getSG(){SousG sg; sg.set(G, vector<int> (size, -1)); return sg;}
    Cost solve();
    Cost follow_order(Tab S);
    Cost contract(int i, SousG& sg);
    void display_order();

    void init(string file);
    void execfile(string file);
    void execdir(string dir);
};

#endif