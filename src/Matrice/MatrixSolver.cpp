#include "MatrixSolver.hpp"
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <chrono>

using namespace std;

void MatrixSolver::solveSansMemo(){
    all_perm(RefMatrix.size, 0, {}, RefMatrix.network);
}

/**
 * @brief generates all permutations recursively
 * 
 * @param n the depth (amount of remaining tensors)
 * @param cost the current cost
 * @param v the current order
 * @param N the current network
 */
void MatrixSolver::all_perm(int n, int cost, vector<pair<int, int>> v, Network N){
    for(int j = 1; j < n; j++){
        for(int i = 0; i < j; i++){
            vector<pair<int, int>> v2 = v;
            v2.push_back(make_pair(i, j));
            all_perm(n-1, cost + contractionCost(i, j, N), v2, contract(i, j, N));
        }
    }
    if(n <= 1 && cost < bestCost){
        bestCost = cost;
        bestOrder = v;

        /*cout << "New best cost : " << cost << endl;
        display_best_order();
        cout << endl;
        cout << "--------------" << endl;*/
    }
}

/**
 * @brief initializes the network based on a textfile
 * 
 * @param path a path to a textfile
 */
void MatrixSolver::init(const char* path){
    cout << "------------ Starting initialisation of network ------------" << endl;
    RefMatrix.init(path);
    cout << "------------ Initialisation of network over ------------" << endl;
    bestCost = INT32_MAX;
    bestOrder.resize(RefMatrix.size);
}

/**
 * @brief displays bestOrder
 * 
 */
void MatrixSolver::display_best_order(){
    for(int k = 0; k < bestOrder.size(); k++){
        cout << "(" << bestOrder[k].first << ", " << bestOrder[k].second << ")||";
    }
}

void MatrixSolver::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    cout << "Starting initialisation on : " << file << endl;
    init(path);
    
    cout << "End of initialisation" << endl;
    cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    solver.solveSansMemo();

    cout << "Best cost : " << bestCost << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
}

void MatrixSolver::execdir(const char* dir){
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
            execfile(path);
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