#ifndef ADJALISTSOLVER_HPP
#define ADJALISTSOLVER_HPP
#include "AdjaList.hpp"
#include <vector>
using namespace std;

typedef tuple<Id, Id, Weight> Contraction; //liste d'arrêtes,

class AdjaListSolver{
    public:
        unsigned int bestCost;
        vector<Contraction> bestOrder;
        unsigned int currentCost;
        vector<Contraction> currentOrder;

        TensorNetwork ReseauT; //le réseau de référence

        void display_best_order();
    
        void solveProcheSansMemo();
        void all_perm(int cost, vector<Contraction> v, Network N);
        void follow_order(vector<Id> v);
        void init();
};

#endif