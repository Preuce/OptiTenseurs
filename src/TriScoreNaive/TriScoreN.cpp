#include "TriScoreN.hpp"

Cost NTS::solve(){
    bestCost = 0;
    for(auto& p : R){
        bestCost += contract(p.first);
    }
    return bestCost;
}

Cost NTS::contract(int i){
    int a = C(E[i].first);
    int b = C(E[i].second);

    if(a != b){
        int res = G[size*a + b];
        for(int j = 0; j < size; j++){
            if(a != j){
                res *= max(1, G[size*b + j]);
            }
            if(b != j){ 
                res *= max(1, G[size*a + j]);
            }
        }

        for(int j = 0; j < size; j++){
            G[size*a + j] *= G[size*b + j];
            G[size*b + j] = 0;
            G[size*j + b] = 0;
            G[size*j + a] = G[size*a + j];
        }
        V[b] = a;
        return res;
    }else{
        return 0;
    }
}

int NTS::C(int i){
    if(V[i] == -1){
        return i;
    }else{
        return C(V[i]);
    }
}

double NTS::ratio(int i){
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

void NTS::display_order(){
    for(int i = 0; i < bestOrder.size()-1; i++){
        cout << bestOrder[i] << " - ";
    }
    cout << bestOrder.back() << '\n';
}

void NTS::init(const char* file){
    G.clear();
    E.clear();
    R.clear();
    V.clear();
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
    sort_edges(E);

    for(int i = 0; i < size; i++){
        G[size*i + i] = 0;
    }

    for(int i = 0; i < E.size(); i++){
        R.push_back(make_pair(i, ratio(i)));
    }

    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});

    for(auto& p : R){
        bestOrder.push_back(p.first);
    }
}

void NTS::execfile(const char* file){
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

void NTS::execdir(const char* dir){
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