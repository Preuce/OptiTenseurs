#ifndef TRISCOREMARGIN_HPP
#define TRISCOREMARGIN_HPP
#include "../Components.hpp"

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
    int size;
    int delta;
    vector<pair<int, int>> E;
    vector<pair<int, double>> R;
    Tab G;

    Tab bestOrder;
    Cost bestCost;
    std::chrono::duration<double> time;

    SousG getSG(){SousG sg; sg.set(G, vector<int> (size, -1)); return sg;}
    Cost solve();
    Cost follow_order(Tab S);
    Cost contract(int i, SousG& sg);

    double ratio(int i);

    void init(const char* file);
    void execfile(const char* file);
    void execdir(const char* dir);
};

#endif