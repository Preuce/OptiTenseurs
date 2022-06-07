#include "SimpleG.hpp"

Cost SimpleG::solve(SouG& sg){
    int key = get_key(sg.S);
    SouG sgref = sg; 

    if(sg.S.size() > 1 && C[key] == -1){
        for(int i = 0; i < sgref.S.size(); i++){
            SouG sg2 = sgref; //on le copie
            int bestE = sg2.S[i];
            sg2.S.erase(sg2.S.begin() + i);

            Cost cost = solve(sg2);
            cost += contract(sgref.S[i], sg2);
            if((cost < C[key] || C[key] == -1) && cost > 0){
                C[key] = cost;
                O[key] = bestE;
                sg.G = sg2.G; //mise à jour de SG pour le faire remonter
                sg.V = sg2.V;
            }
        }
    }else if(C[key] == -1){
        C[key] = contract(sg.S[0], sg);
        O[key] = sg.S[0];
    }else{
        //cas où C[key] est déjà calculé
        //on connait l'ensemble des arêtes qui ont été contractées : sg.S
        //on peut bruteforce les S.size() contractions, puisqu'on a déjà le meilleur coût on se fiche de l'ordre
        for(int i : sg.S){
            cheap_contract(i, sg);
        }
        //pas giga opti mais ça passe
    }

    return C[key];
}

/**
 * @brief computes the cost of a contraction and modifies the sub-graph (V and G)
 * 
 * @param i the index (in E) of the contracted edge
 * @param sg the sub-graph
 * @return Cost 
 */
Cost SimpleG::contract(int i, SouG& sg){
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
 * @brief same as contract, but doesn't compute the cost (only updates G and V)
 * 
 * @param i 
 * @param sg 
 */
void SimpleG::cheap_contract(int i, SouG& sg){
    int a = sg.C(E[i].first);
    int b = sg.C(E[i].second);

    if(a != b){
        for(int j = 0; j < size; j++){
            sg.G[size*a + j] *= sg.G[size*b + j];
            sg.G[size*b + j] = 0;
            sg.G[size*j + b] = 0;
            sg.G[size*j + a] = sg.G[size*a + j];
        }
        sg.V[b] = a;
    }
}

/**
 * @brief retrieves the vertex thats is the reference for i in the graph
 * 
 * @param i a vertex
 * @return int 
 */
int SouG::C(int i){
    while(V[i] != -1){
        i = V[i];
    }
    return i;
}

/**
 * @brief encodes a set of edges as a unique int key
 * 
 * @param S 
 * @return int 
 */
int SimpleG::get_key(Tab S){
    int res = 0;
    for(int i : S){
        res += pow(2, i);
    }
    return res-1;
}

/**
 * @brief displays the best order given a starting state encoded as a key
 * 
 * @param key 
 */
void SimpleG::display_order(int key){
    int i = O[key]; //i = la meilleure arête à contracter pour l'état key-2^i + 1
    int next = key-pow(2, i);
    if(key == get_key(S)){
        display_order(next);
        cout << i << '\n';
    }else if(next >= 0){
        display_order(next);
        cout << i << " - ";
    }else if(next == -1){
        cout << i << " - ";
    }
}

void SimpleG::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    E.clear();
    O.clear();
    C.clear();
    S.clear();

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                G.resize(size*size, 1);
                S.resize(3*size/2 - 2);
                O.resize(pow(2, 3*size/2-2)-1, -1);
                C.resize(pow(2, 3*size/2-2)-1, -1);
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
        S[i] = i;
    }

    sgref = getSG();
}

void SimpleG::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    int c = solve(sgref);
    cout << "Best cost : " << c << endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    cout << "Best order : ";
    display_order(get_key(S));
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
}

void SimpleG::execdir(const char* dir){
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