#ifndef CLASSESSOLVER_HPP
#define CLASSESSOLVER_HPP
#include "Classes.hpp"

typedef pair<Id, Id> Contraction;
typedef vector<Contraction> Ordre;

void display_all(Network N);

class ClassesSolver{
    public:
    unsigned int bestCost;
    Ordre bestOrder;

    vector<Id> stillUp;

    ClassesNetwork RefNetwork;

    void solve();
    void all_perm(int cost, Network N, Ordre o, vector<Id> stillUp);
    void display_best_order();
    void execfile(const char* file);
    void execdir(const char* dir);
};

ClassesSolver solver;
#endif