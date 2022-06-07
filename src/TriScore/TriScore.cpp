#include "TriScore.hpp"

/**
 * @brief solves G
 * 
 * @return int 
 */
Cost TriScore::solve(){
    int cost = 0;
    //tant qu'il reste des arètes
    while(!R.empty()){
        bestOrder.push_back(R.front().first);
        int i = C(E[R.front().first].first);
        int k = C(E[R.front().first].second);
        if(i != k){
            //ajoute le coût du retrait de R.front à cost
            cost += ext_cost(i, k)*G[size*i + k];
            contract(i, k);
        }
        R.erase(R.begin());

        //update de R
        updateRatio();
    }
    return cost;
}

/**
 * @brief mostly equivalent to sortRatio, updates the sorted list of scores R
 * It is sub-optimal to sort the entire list, especially considering that we have 3-4 edges to move at best
 * but at the same time, 95% of the list is already in order so its not that bad
 */
void TriScore::updateRatio(){
    for(auto& p : R){
        int i = C(E[p.first].first);
        int k = C(E[p.first].second);
        p.second = G[size*i + k]/(float) ext_cost(i, k);
    }
    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

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

void TriScore::init(const char* file){
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

void TriScore::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    bestCost = solve();
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    cout << "Best cost : " << bestCost << '\n';
    cout << "Best order : ";
    display_order();
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void TriScore::execdir(const char* dir){
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
            //cout << "FILE : " << path << endl;
            display(path);
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}
