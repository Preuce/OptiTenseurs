#ifndef TRISCOREN_HPP
#define TRISCOREN_HPP
#include "../Components.hpp"

/**
 * @brief Version alternative naïve de TriScore
 * L'ordre de contraction est calculée au départ et n'est jamais mis à jour
 */

class NTS /*: public Algorithm*/{
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

    void init(string file);
    void execfile(string file);
    void execdir(string file);
};
#endif