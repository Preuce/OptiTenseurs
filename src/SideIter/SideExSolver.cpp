#include "SideExSolver.hpp"

Cost SideIter::solve(){
    pair<int, int> p;
    //pour commencer, T est {R1, Q1, N1, 1, 1, ..., 1}, C est fill à 0
    //passage de l'état s, à s+1
    for(int s = 0; s < size/2-1; s++){
        int N = G[size*s + s+size/2];
        int ofs = 2*s;
        int ofsc = (s+1)*(s+1)-1;
        for(int k = 2*(s+1); k >= 2; k--){
            //pour chaque produits de Nk, on calcule le coût de contracter vers l'arête centrale suivante
            T[k] = T[k-2]*N;
            C[k] = C[k-2] + contract(s, k, 2, p);

            O[ofsc + k] = p;

            int Rs = C[k-2] + contract(s, k, 0, p);

            //celui qui donne le meilleur coût pour R est aussi celui qui donne le meilleur coût pour Q
            if((Rs < P[ofs] || P[ofs] == -1) && Rs > 0){
                P[ofs] = Rs; //minimum de (coût d'arrivée + coût de sortie) pour R à l'état s
                Z[s] = k; //arête centrale donnant le meilleur coût R[s]
                O[ofsc] = p; //meilleure ordre de contraction

                int Qs = C[k-2] + contract(s, k, 1, p);
                P[ofs+1] = Qs;
                O[ofsc + 1] = p; 
            }
            
            //restoreA(s); //inutile ?
        }
        T[0] = G[size*s + s+1];
        T[1] = G[size*(s+size/2) + s+1+size/2];
        C[0] = P[ofs];
        C[1] = P[ofs+1];
    }

    //gestion de la dernière étape, on récupère directement le résultat
    int s = size/2-1;
    int N = G[size*s + s+size/2];
    int cost = INT32_MAX;

    for(int k = 0; k <= 2*s; k++){
        int ck = C[k] + T[k]*N;
        if(ck < cost && ck > 0){
            cost = ck;
            Z[s] = k; //donne le k de l'avant-dernier état qui mène à l'optimum
        }
    }
    return cost; 
}

/**
 * @brief computes the contraction cost
 * 
 * @param s the state we are currently at
 * @param k the index in T of the tensor t we just calc'ed
 * @param x the contraction
 * @param p a buffer that stores the exact order of contraction
 * @return Cost 
 */
Cost SideIter::contract(int s, int k, int x, pair<int, int>& p){
    computeA(s, k);
    Cost G1 = G[size*(s+size/2) + s+1+size/2];
    Cost G0 = G[size*s + s+1];

    Cost r12;
    Cost r21;

    Cost r02;
    Cost r20;
    Cost cost;
    switch(x){ //c'est là qu'on return le min dans les 2 ordres possibles
        //cas arête du dessus
        //o-R- (G0)
        //N
        //o-Q- (G1)
        case 0:
            //Q puis N
            r12 = A[s+1+size/2]*T[k] + A[s]*A[s+1+size/2]/G1;

            //N puis Q
            r21 = G0*G1*T[k] + G0*A[s+1+size/2];
            if(r12 < r21){
                cost = r12;
                p = make_pair(1, 2);
            }else{
                cost = r21;
                p = make_pair(2, 1);
            }
            break;
        //cas arête du dessous
        case 1:
            r02 = A[s+1]*T[k] + A[s+size/2]*A[s+1]/G0;
            r20 = G1*(G0*T[k] + A[s+1]);
            if(r02 < r20){
                cost = r02;
                p = make_pair(0, 2);
            }else{
                cost = r20;
                p = make_pair(2, 0);
            }
            break;
        //cas arète centrale
        case 2:
            cost = T[k]*(A[s+1] + A[s+1+size/2]);
            p = make_pair(0, 1);
            break;
        default:
            cost = 0;
            break;
    }
    restoreA(s);
    return cost;
}

/**
 * @brief computes A, the outer weights of each tensor in a given state
 * 
 * @param k the index int T of the tensor t we just calc'ed
 * @param s the state we are currently at
 */
void SideIter::computeA(int s, int k){
    A[s] = G[size*s + s+1]*T[k];
    A[s+size/2] = G[size*(s+size/2) + s+1+size/2]*T[k];
}

/**
 * @brief restores A to a given state
 * 
 * @param s the state
 */
void SideIter::restoreA(int s){
    A[s] = G[size*size + s]; //stock le poids sortant du sommet s
    A[s+size/2] = G[size*size + s+size/2];
}

/**
 * @brief displays the best contraction order using backtracking
 * 
 * @param s a state
 * @param k the center-edge that lead to the best final cost
 */
void SideIter::display_order(int s, int k){
    int ofs = (s+1)*(s+1)-1;
    if(s >= 0){
        if(k > 1){
            display_order(s-1, k-2);
        }else{
            display_order(s-1, Z[s]-2);
        }
        cout << "|" << O.at(ofs + k).first << " - " << O.at(ofs + k).second << "|";
    }
}

void SideIter::display_order(){
    for(int i = 0; i < bestOrder.size()-1; i++){
        cout << bestOrder[i] << " - ";
    }
    cout << bestOrder.back() << '\n';
}

void SideIter::get_order(int s, int k){
    int ofs = (s+1)*(s+1)-1;
    if(s >= 0){
        if(k > 1){
            get_order(s-1, k-2);
        }else{
            get_order(s-1, Z[s]-2);
        }
        int e1 = O[ofs + k].first;
        int e2 = O[ofs + k].second;

        switch(e1){
            case 0:
                bestOrder.push_back(s);
            break;
            case 2:
                bestOrder.push_back(size/2-1 + s);
            break;
            case 1:
                bestOrder.push_back(size-1+s);
            break;
        }

        switch(e2){
            case 0:
                bestOrder.push_back(s);
            break;
            case 2:
                bestOrder.push_back(size/2-1 + s);
            break;
            case 1:
                bestOrder.push_back(size-1 + s);
            break;
        }
        //cout << "|" << O.at(ofs + k).first << " - " << O.at(ofs + k).second << "|";
    }
}

void SideIter::init(const char* file){
    G.clear();
    A.clear();
    P.clear();
    C.clear();
    T.clear();
    O.clear();
    Z.clear();
    bestOrder.clear();

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                G.resize(size*(size+1), 1);
                A.resize(size, 1);
                P.resize(size, -1);
                C.resize(2*(size/2)-1, 0); //tableau des coûts
                T.resize(2*(size/2)-1, 1); //tableau des t
                O.resize(size*size/4, {-1, -1});
                Z.resize(size/2, -1);
            break;
            case 'e':
                flux >> i >> j >> w;
                G[size*i + j] = w;
                G[size*j + i] = w;
                G[size*size + i] *= w;
                G[size*size + j] *= w;
                A[i] *= w;
                A[j] *= w;
            break;
            default:
            break;
        }
    }
}

void SideIter::execfile(const char* file){
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
    get_order(size/2-2, Z[size/2-1]);    
    bestOrder.push_back(size-2);
    cout << "Best order : ";
    display_order();
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void SideIter::execdir(const char* dir){
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
            cout << "FILE : " << path << endl;
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}