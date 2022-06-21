#include "SplitSolver.hpp"

/**
 * @brief Solves a given state
 * 
 * @param S The tensors in this state
 * @return int the best cost for S
 */
Cost Split::solve(Tab S){
    //encodage de l'ensemble de sommets
    long int key = convert(S);

    //si il reste plus d'1 sommet et que le coût n'a pas encore été calculé
    if(C[key] == -1 && S.size() > 1){
        int cost;
        
        Tab S1;
        Tab S2;

        Cost cout_sortant = produit_sortant(S, computeA(S));
        
        //pour toutes les séparations de S
        do{
            //on déplace la "barre" où on fait la coupure S1/S2 dans S
            for(int i = 0; i < S.size() - 1; i++){
                S1.clear();
                S2.clear();
                //on attribue les sommets de S1
                for(int k = 0; k <= i; k++){
                    S1.push_back(S[k]);
                }
                //on attribue les sommets de S2
                for(int k = i+1; k < S.size(); k++){
                    S2.push_back(S[k]);
                }
                cost = solve(S1) + solve(S2) + cout_sortant*cut(S1, S2);
                if(cost < C[key] && cost > 0|| C[key] == -1){
                    C[key] = cost;
                    P1[key] = convert(S1);
                    P2[key] = convert(S2);
                }
            }
        }while(next_permutation(S.begin(), S.end()));
    }else if(S.size() == 1){
        C[key] = 0;
    }
    return C[key];
}

/**
 * @brief computes the S.size()-1'th column of A
 * 
 * @param S The tensors in this state
 * @return Tab an updated copy of A
 */
Tab Split::computeA(Tab S){
    for(int i : S){
        A[size*(S.size()-1)+i] = G[size*size + i];
        for(int k : S){
            A[size*(S.size()-1)+i] /= G[size*i + k];
        }   
    }
    return A;
}

/**
 * @brief computes the product of all edges linking 2 states together
 * 
 * @param S1 A state
 * @param S2 A state
 * @return int 
 */
Cost Split::cut(Tab S1, Tab S2){
    Cost res = 1;
    for(int i : S1){
        for(int j : S2){
            res *= G[size*i + j];
        }
    }
    return res;
}

/**
 * @brief computes the product of all the edges leaving a state
 * 
 * @param S The tensors in this state
 * @param A The external-weights of all tensors for each states
 * @return int 
 */
Cost Split::produit_sortant(Tab S, Tab A){
    Cost res = 1;
    for(int i : S){
        res *= A[size*(S.size()-1) + i];
    }
    return res;
}

/**
 * @brief converts a state in a unique integer key
 * 
 * @param S The tensors in this state
 * @return int 
 */
long int Split::convert(Tab S){
    int res = 0;
    for(int i : S){
        res += pow(2, i);
    }
    return res;
}

/**
 * @brief converts a key into a set of tensors
 * 
 * @param key a code generated from a state using convert(S)
 * @return Tab 
 */
Tab Split::recover(long int key){
    Tab res;
    for(int i = size; i >= 0; i--){
        int p = pow(2, i);
        if(key >= p){
            res.push_back(i);
            key -= p;
        }
    }
    return res;
}

void Split::display_order(Tab S){//dégueulasse
    if(S.size() > 1){
        int key = convert(S);
        display_order(recover(P1[key]));
        display_order(recover(P2[key]));
        cout << "| ";
        for(int i : S){
            cout << i << " | ";
        }
        cout << endl;
    }
}

void Split::init(string file){
    cout << "Clearing" << '\n';
    S.clear();
    G.clear();
    A.clear();
    C.clear();
    P1.clear();
    P2.clear();
    cout << "End of clear" << '\n';

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                cout << "Start of resize" << '\n';
                size = atoi(&line[2]);
                
                //G.resize(size*size, 1);*
                cout << "S" << '\n';
                S.resize(size);
                cout << "G" <<'\n';
                G.resize(size*(size+1), 1);
                cout << "A" << '\n';
                A.resize(size*size, 1);
                cout << "C" << '\n';
                C.resize(pow(2, size), -1);
                cout << "P" << '\n';
                P1.resize(pow(2, size), -1);
                P2.resize(pow(2,size), -1);
                cout << "End of resize" << "\n";
            break;
            case 'e':
                flux >> i >> j >> w;
                G[size*i + j] = w;
                G[size*j + i] = w;
                G[size*size + i] *= w;
                G[size*size + j] *= w;
            break;
            default:
            break;
        }
    }
    cout << "End of import" << '\n';

    for(int k = 0; k < size; k ++){
        S[k] = k;
    }
    cout << size << '\n';
}

void Split::execfile(string file){
    string path = "../instances/" + file;
    cout << "Starting initialisation on : " << file << '\n';
    init(path);
    cout << "End of initialisation" << '\n';
    cout << "Starting solving" << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    bestCost = solve(S);
    cout << "End of solving" << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    cout << "Best cost* : " << bestCost << '\n';
    cout << "Best order : " << '\n';
    display_order(S);
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void Split::execdir(string dir){
    string base = "../instances/" + dir + "/";
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(base.c_str());
    if(dp == NULL){
        cerr << "Could not open directory : " << base << '\n';
        exit(-1);
    }
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            string path = base + file->d_name;
            display(path);
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}

