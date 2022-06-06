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
    void solveAlt(SouG sg, Tab& A);

    int size;
    vector<pair<int, int>> E; //liste des arêtes (fixe)
    Tab G; //size*(size+1)
    Tab S;
    Tab O;

    SouG getSG(){SouG sg; sg.set(S, G, vector<int> (size, -1)); return sg;}
    Cost contract(int i, SouG& sg);

    Cost solve(SouG sg);

    Tab computeA(Tab S);
    Cost follow_order(Tab S);
    void display_order();

    void get_size(char* preamble);
    void init(const char* file);

    void execfile(const char* file);
    void execdir(const char* dir);
};
#endif
