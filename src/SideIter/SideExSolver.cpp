#include "SideExSolver.hpp"

Cost SideIter::solve(){
    pair<int, int> p;
    //passage de l'état s, à s+1
    for(int s = 0; s < size/2-1; s++){
        int N = G[size*s + s+size/2]; //arête centrale à multiplier
        int ofs = 2*s; //offset dans le tableau T
        int ofsc = (s+1)*(s+1)-1; //offset dans le tableau C
        for(int k = 2*(s+1); k >= 2; k--){ //à chaque étape, on considère 2 possibilités
            //pour chaque produits de Nk, on calcule le coût de garder l'arête
            T[k] = T[k-2]*N;

            //coût de contracter les 2 autres arêtes (dans le meilleur ordre possible)
            C[k] = C[k-2] + contract(s, k, 2, p);

            //stockage de l'ordre ayant donné le coût le plus faible
            O[ofsc + k] = p;

            //coût de garder l'arête du haut, sachant que l'arête centrale est de poids T[k] et a coûté C[k-2]
            int Rs = C[k-2] + contract(s, k, 0, p);

            //si cette valeur d'arête centrale offre les meilleurs contractions pour les arêtes latérale, on le garde en mémoire
            if((Rs < P[ofs] || P[ofs] == -1) && Rs > 0){
                //Rs = minimum de (coût d'arrivée + coût de sortie) pour R à l'état s
                //P[ofs] stock le meilleur coût Rs obtenu pour l'instant
                P[ofs] = Rs;
                //on stock l'arête centrale donnant le meilleur coût pour R et Q à l'état s 
                Z[s] = k; 
                //on stock l'ordre (la paire d'arête) dans lequel on a contracté
                O[ofsc] = p; 

                //l'arête centrale donnant le meilleur coût pour R est aussi celle donnant le meilleur coût pour Q
                int Qs = C[k-2] + contract(s, k, 1, p);
                P[ofs+1] = Qs;
                O[ofsc + 1] = p; 
            }
        }
        //on ajoute les 2 nouvelles arêtes latérale
        T[0] = G[size*s + s+1];
        T[1] = G[size*(s+size/2) + s+1+size/2];

        //on range le meilleur coût pour R et Q
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
            //donne le k de l'avant-dernier état qui mène à l'optimum, afin de le récupérer facilement
            Z[s] = k; 
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
    //on calcule les poids sortants des sommets s et s+D
    computeA(s, k);
    //on récupère les poids des arêtes latérales (pas nécessaire mais plus lisible)
    Cost G1 = G[size*(s+size/2) + s+1+size/2];
    Cost G0 = G[size*s + s+1];

    //les coûts associés aux différents ordres de contraction
    Cost r12;
    Cost r21;

    Cost r02;
    Cost r20;
    Cost cost;
    switch(x){
        //o-R- (G0)
        //N
        //o-Q- (G1)
        
        //on garde l'arête du dessus (R)
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
        //cas arête du dessous (Q)
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
        //cas arête centrale
        case 2:
            cost = T[k]*(A[s+1] + A[s+1+size/2]);
            p = make_pair(0, 1);
            break;
        default:
            cost = 0;
            break;
    }
    //on remet le tableau des poids sortants à son état initial (au cas où)
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
    //pour s et s+D, on ignore l'arête "de derrière"
    A[s] = G[size*s + s+1]*T[k];
    A[s+size/2] = G[size*(s+size/2) + s+1+size/2]*T[k];
}

/**
 * @brief restores A to a given state
 * 
 * @param s the state
 */
void SideIter::restoreA(int s){
    A[s] = G[size*size + s];
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

void SideIter::init(string file){
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

Cost SideIter::call_solve(){
    Cost c = solve();
    get_order(size/2-2, Z[size/2-1]);    
    bestOrder.push_back(size-2);
    return c;
}