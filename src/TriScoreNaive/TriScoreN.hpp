#ifndef TRISCOREN_HPP
#define TRISCOREN_HPP
#include "../Components.hpp"

class NTS{
    public:
    int size;
    Tab G;
    Tab V;
    vector<pair<int, int>> E;
    vector<pair<int, double>> R;

    int bestCost;
    Tab bestOrder;
    std::chrono::duration<double> time;

    Cost solve();
    int C(int i);
    Cost contract(int i);
    double ratio(int i);

    void display_order();

    void init(const char* file);
    void execfile(const char* file);
    void execdir(const char* file);
};
#endif