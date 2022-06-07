#ifndef SIMPLEG_HPP
#define SIMPLEG_HPP
#include "../Components.hpp"

class SouG{
    public:
    Tab S;
    Tab G;
    Tab V;

    int C(int i);
    void set(Tab S, Tab G, Tab V){this->S = S; this->G = G; this->V = V;};
};

class SimpleG{
    public:

    int size;
    SouG sgref;
    vector<pair<int, int>> E; //liste des arêtes (fixe)
    Tab G; //size*(size+1)
    Tab S;
    Tab O;
    Tab C;

    Cost bestCost;
    Tab bestOrder;
    std::chrono::duration<double> time;

    SouG getSG(){SouG sg; sg.set(S, G, vector<int> (size, -1)); return sg;}
    Cost contract(int i, SouG& sg);
    void cheap_contract(int i, SouG& sg);

    Cost solve(SouG& sg);

    int get_key(Tab S);
    Tab get_Tab(int key);

    void display_order(int key);
    void display_order();
    void get_order(int key);

    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif
