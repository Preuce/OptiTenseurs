#include "ClassesSolver.hpp"
#include <iostream>
#include <string.h>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>

void ClassesSolver::solve(){
    all_perm(0, RefNetwork.getNetwork(), {}, stillUp);
}

void ClassesSolver::all_perm(int cost, Network N, Ordre o, vector<Id> stillUp){
    //display_all(N);
    //cout << *k << endl;
    //cout << "-----------" <<endl << "-----------" << endl;
    for(int k = 0; k < stillUp.size(); k++){
        int i = stillUp[k];
        for(pair<Id, Weight> p : N[i].voisins){
            int j = p.first;
            //coût
            
            //ordre
            //contracter + suivant
            Ordre o2 = o;
            o2.push_back(make_pair(i, j)); //TODO:
            vector<Id> newUp = stillUp;
            newUp.erase(newUp.begin() + k); //TODO:
            //TODO: memo
            all_perm(cost + contractionCost(i, j, N), contract(j, i, N), o2, newUp);
        }
    }
    if(stillUp.size() == 1 && cost < bestCost){
        bestCost = cost;
        bestOrder = o;

        /*cout << "New best cost : " << cost << endl;
        display_best_order();
        cout << endl;
        cout << "--------------" << endl;*/
    }

}

void display_all(Network N){
    for(Classe c : N){
        cout << "Classe " << c.sommets[0] << " : " << endl;
        cout << "{";
        for(Id s : c.sommets){
            cout << s << ", ";
        }
        cout << "}" << endl;
        for(pair<Id, Weight> v : c.voisins){
            cout << v.first << " : " << v.second << endl;
        }
        cout << "---------------" << endl;
    }
}

void ClassesSolver::display_best_order(){
    for(Contraction c : bestOrder){
        cout << "|(" << c.first << ", " << c.second << ")|";
    }
}

void ClassesSolver::execfile(const char* file){
    RefNetwork.init(file);
    bestOrder.clear();
    bestCost = INT32_MAX;
    for(int i = 0; i < RefNetwork.getNetwork().size(); i++){
        stillUp.push_back(i);
    }
    auto start = std::chrono::high_resolution_clock::now();
    solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "Best cost : " << bestCost << endl;
}

void ClassesSolver::execdir(const char* dir){
    char base[100] = "../instances/";
    strcat(base, dir);
    strcat(base, "/");
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(base);
    
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            char path[100];
            strcpy(path, base);
            strcat(path, file->d_name);
            solver.execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}

int main(int argc, char* argv[]){
    switch(argc){
        case 1:
            cout << "Missing argument : instance file" << endl;
            return -1;
            break;
        case 2:
            solver.execfile(argv[1]);
            break;
        case 3:
            if(*argv[1] == 'd'){
                solver.execdir(argv[2]);
            }
            break;
        default:
            cout << "Call error : wrong argument" << endl;
            return -1;
            break;
    }
    return 0;
}