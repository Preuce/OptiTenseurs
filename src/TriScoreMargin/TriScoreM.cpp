#include "TriScoreM.hpp"

Cost TriScoreM::solve(){
    //TriScore a été exécuté à l'initialisation de l'instance
    //on obtient une borne supérieure au meilleur coût, et un ordre de référence
    Tab refOrder = bestOrder;

    for(int d = 1; d <= delta; d++){
        for(int i = 0; i < E.size()-d; i++){
            //copie de l'ordre de référence
            Tab S (refOrder);
            int n = S[i];
            //modification de l'ordre
            for(int k = i; k < i+d; k++){
                S[k] = S[k+1];
            }
            S[i+d] = n;
            Cost cost = follow_order(S);
            if(cost > 0 && (cost < bestCost || bestCost == -1)){
                bestCost = cost;
                bestOrder = S;
            }
        }
    }
    return bestCost;
}

/**
 * @brief Computes a cost given an order of contraction to follow
 * 
 * @param S the list of edges to contract
 * @return Cost 
 */
Cost TriScoreM::follow_order(Tab S){
    SousG sg = getSG();
    Cost cost = 0;
    for(int i : S){
        cost += contract(i, sg);
        if(cost >= bestCost && bestCost != -1){return -1;}
    }
    return cost;
}

/**
 * @brief Computes the cost of contracting an edge i in the graph G, and updates G and V
 * 
 * @param i 
 * @param sg a sub-graph
 * @return Cost 
 */
Cost TriScoreM::contract(int i, SousG &sg){
    int a = sg.C(E[i].first);
    int b = sg.C(E[i].second);

    if(a != b){
        int res = sg.G[size*a + b];
        for(int j = 0; j < size; j++){
            if(a != j){
                res *= max(1, sg.G[size*b + j]);
            }
            if(b != j){ 
                res *= max(1, sg.G[size*a + j]);
            }
        }

        for(int j = 0; j < size; j++){
            sg.G[size*a + j] *= sg.G[size*b + j];
            sg.G[size*b + j] = 0;
            sg.G[size*j + b] = 0;
            sg.G[size*j + a] = sg.G[size*a + j];
        }
        sg.V[b] = a;
        return res;
    }else{
        return 0;
    }
}

/**
 * @brief returns i's vertex of reference in the graph
 * 
 * @param i 
 * @return int 
 */
int SousG::C(int i){
    while(V[i] != -1){
        i = V[i];
    }
    return i;
}

void TriScoreM::display_order(){
    for(int i = 0; i < bestOrder.size()-1; i++){
        cout << bestOrder[i] << " - ";
    }
    cout << bestOrder.back() << '\n';
}

void TriScoreM::init(string file){
    triscore.init(file);

    G.clear();
    E.clear();
    bestCost = -1;
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
                delta = min(max(refdelta, 1), 3*size/2 - 2);
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

    bestCost = triscore.solve();
    bestOrder = triscore.bestOrder;

    for(int i = 0; i < size; i ++){
        G[size*i + i] = 0;
    }

    sort_edges(E);
}

Cost TriScoreM::call_solve(){
    return solve();
}