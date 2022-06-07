#include "TriScoreM.hpp"

Cost TriScoreM::solve(){
    //bestCost = TriScore.solve();
    //on obtient une borne supérieure au meilleur coût, et un ordre de référence
    //on forme R sur cet ordre wj/ext_cost(j)
    Tab O;
    for(auto& p : R){
        //cout << p .first << '\n';
        O.push_back(p.first);
    }
    //on récupère uniquement les indices par simplicité
    do{
        Cost cost = follow_order(O);
        if(cost > 0 && (cost < bestCost || bestCost == -1)){
            bestCost = cost;
            bestOrder = O; //je pense pas que ça marche comme ça malheureusement
        }
    }while(next_permutation(O.begin(), O.end()));
    return bestCost;
}

Cost TriScoreM::follow_order(Tab S){
    SousG sg = getSG();
    Cost cost = 0;
    for(int i : S){
        cost += contract(i, sg);
        if(cost >= bestCost && bestCost != -1){return -1;}
    }
    return cost;
}

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

int SousG::C(int i){
    while(V[i] != -1){
        i = V[i];
    }
    return i;
}

double TriScoreM::ratio(int i){
    int a = E[i].first;
    int b = E[i].second;
    int res = 1;
    for(int j = 0; j < size; j++){
        if(a != j){
            res *= max(1, G[size*b + j]);
        }
        if(b != j){ 
            res *= max(1, G[size*a + j]);
        }
    }

    return G[size*a + b]/(double) res;
}

void TriScoreM::init(const char* file){
    G.clear();
    E.clear();
    R.clear();
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
                delta = min(max(delta, 0), 3*size/2 - 2);
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
        R.push_back(make_pair(i, ratio(i)));
    }

    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});

}

void TriScoreM::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    cout << "Range : " << delta << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    bestCost = solve();
    cout << "Best cost : " << bestCost << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    //cout << "Best order : ";
    //display_order(S);
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << std::endl;
    cout << "--------------" << endl;
    //delta = -1;
}

void TriScoreM::execdir(const char* dir){
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
            display(path);
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}