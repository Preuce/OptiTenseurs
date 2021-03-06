#include "TriScore.hpp"

/**
 * @brief solves G
 * 
 * @return int 
 */
Cost TriScore::solve(){
    bestCost = 0;
    //tant qu'il reste des arêtes
    while(!R.empty()){
        bestOrder.push_back(R.front().first);
        //les vraies extrémités de l'arête
        int i = C(E[R.front().first].first);
        int k = C(E[R.front().first].second);
        if(i != k){
            //ajoute le coût de contraction de R.front à cost
            bestCost += ext_cost(i, k)*G[size*i + k];
            contract(i, k);
        }
        //suppression de l'arête
        R.erase(R.begin());

        //update du tableau des ratio
        updateRatio();
    }
    return bestCost;
}

/**
 * @brief updates the sorted list of scores R
 * Sub-optimal, but time is not an issue here
 */
void TriScore::updateRatio(){
    for(auto& p : R){
        int i = C(E[p.first].first);
        int k = C(E[p.first].second);
        p.second = G[size*i + k]/(float) ext_cost(i, k);
    }
    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

/**
 * @brief Computes the external cost of 2 vertex
 * 
 * @param i 
 * @param k 
 * @return Cost 
 */
Cost TriScore::ext_cost(int i, int k){
    int res = 1;
    for(int j = 0; j < size; j++){
        if(k != j){
            res *= max(1, G[size*i + j]);
        }
        if(i != j){ 
            res *= max(1, G[size*k + j]);
        }
    }

    return res;
}

/**
 * @brief contracts an edge by modifying G and V accordingly
 * 
 * @param i
 * @param k
 */
void TriScore::contract(int i, int k){
    for(int j = 0; j < size; j++){
        G[size*i + j] *= G[size*k + j];
        G[size*k + j] = 0;
        G[size*j + k] = 0;
        G[size*j + i] = G[size*i + j];
    }
    V[k] = i;
}

/**
 * @brief returns i's vertex of reference in the graph
 * 
 * @param i 
 * @return int 
 */
int TriScore::C(int i){
    if(V[i] == -1){
        return i;
    }else{
        return C(V[i]);
    }
}

/**
 * @brief displays O, the order followed by the heuristic
 * 
 */
void TriScore::display_order(){
    for(int i = 0; i < bestOrder.size()-1; i++){
        cout << bestOrder[i] << " - ";
    }
    cout << bestOrder.back() << '\n';
}

void TriScore::init(string file){
    G.clear();
    R.clear();
    V.clear();
    E.clear();
    bestOrder.clear();

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                G.resize(size*size, 1);
                V.resize(size, -1);
            break;
            case 'e':
                flux >> i >> j >> w;
                E.push_back(make_pair(i, j));
                G[size*i + j] = w;
                G[size*j + i] = w;
            break;
            default:
            break;
        }
    }

    for(int i = 0; i < size; i ++){
        G[size*i + i] = 0;
    }

    sort_edges(E);

    for(int i = 0; i < E.size(); i++){
        int j = E[i].first;
        int k = E[i].second;
        R.push_back(make_pair(i, G[size*j  + k]/ (float) ext_cost(j, k)));
    }

    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

Cost TriScore::call_solve(){
    return solve();
}

void get_approx_solution(int& cost, Tab& O, string file){
    TriScore solver;
    solver.init(file.c_str());
    solver.solve();
    cost = solver.bestCost;
    O = solver.bestOrder;
}