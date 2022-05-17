#include "AdjaListSolver.hpp"
#include <bits/stdc++.h>

using namespace std;

void AdjaListSolver::solveProcheSansMemo(){

    all_perm(0, {}, ReseauT.getTensors());
}

void AdjaListSolver::all_perm(int cost, vector<Contraction> v, Network N){
    if(N.size() > 1){
        for(int j = 0; j < N.size(); j++){
            for(pair<Id, Weight> p : N[j]){
                Network N2 = N;
               
                vector<Contraction> v2 = v;
                pair<int, Network> t = contract(j, p.first, N2);
               
                v2.push_back(make_tuple(j, p.first, p.second));
               
                all_perm(cost + t.first, v2, t.second);
            }
        }
    }else if(cost < bestCost){
        bestCost = cost;
        bestOrder = v;

        cout << "New best cost : " << cost << endl;
        display_best_order();
        cout << endl;
        cout << "--------------" << endl;
    }
}

void AdjaListSolver::display_best_order(){
    for(Contraction c : bestOrder){
        cout << "|(" << get<0> (c) << " " << get<1> (c) << " " << get<2> (c) << ")|";
    }
    cout << endl;
}

void AdjaListSolver::init(){
    ReseauT.init("../instances/test.txt");
    bestCost = INT32_MAX;
    bestOrder.clear();

}

int main(int argc, char* argv[]){
    AdjaListSolver solver;
    cout << "------------ Starting initialisation of solver ------------" << endl;
    solver.init();
    cout << "------------ Initialisation of solver over ------------" << endl;
    cout << "------------ Starting solving ------------" << endl;
    solver.solveProcheSansMemo();
    cout << "------------ Solving finished ------------" << endl;
    cout << "Best order :" << endl;
    solver.display_best_order();
    cout << endl;
    cout << "Best cost : " << solver.bestCost << endl;

    return 0;
}