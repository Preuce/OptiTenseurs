#include "SplitSmart.hpp"

Cost SS::solve(int i1, int i2, int j1, int j2){
    //objectifs : limiter le nombre d'arêtes sortantes à delta
    //limiter le chevauchement/la redondance
 
    if(){
        //on a un sommet seul, on ne peut plus split
        //return 0
    }else{
        for(/*range de i1*/){
            for(/*range de i2*/){
                for(/*range de j1*/){
                    for(/*range de j2*/){
                        //on doit récupérer les 2 sous-ensembles de sommets ou bien d'arêtes
                        //on doit récupérer le coût de coupure
                        //on doit récupérer le nombre d'arêtes coupées et continue; en conséquence
                        Cost c = 0;
                        Cost CQ = *C_at(x1,x2,y1,y2);
                        if(CQ == -1){
                            c += solve(x1,x2,y1,y2);
                        }
                        if(CQ == /*trop grand*/){
                            continue;
                        }

                        int u1, u2, v1, v2;
                        if(i1 == x1){ //TODO: pas un branchement si possible
                            u1 = x2;
                            u2 = i2;
                        }else{
                            u1 = i1;
                            u2 = x1;
                        }
                        if(j1 == y1){
                            v1 = y2;
                            v2 = j2;
                        }else{
                            v1 = j1;
                            v2 = y1;
                        }

                        Cost CR = *C_at(u1, u2, v1, v2);
                        
                        if(CR == -1){
                            c += solve(/*le reste*/);
                        }

                        if(CR == /*trop grand*/){
                            continue;
                        }

                        //on note S l'ensemble de sommets formé par i1, i2, j1, j2
                        //calculer A(S) : computeA(S);
                        
                        //cost = solve(Q) + solve(R) + A(S)*cut(Q,R)
                        //if(cost > 0)
                        //  if(*C_at(i1,i2,j1,j2) == -1 || cost < C_at(i1,i2,j1,j2))
                        //      *C_at(i1,i2,j1,j2) = cost;
                        // elif(cost > bestCost)
                        //      *C_at(i1,i2,j1,j2) = tropgrand
                    }
                }
            }
        }
    }
}

/**
 * @brief computes the outer-cost
 * 
 * @param S the set of edges we splitted on
 */
void SS::computeA(Tab S){
    
}

Cost SS::solve_Bloc(int i1, int i2, int j1, int j2){
    int s = i2-i1 + j2-j1 + 2; //nombre de sommets dans l'ensemble
    //avec le nombre de sommet, je connais la taille de S et je peux aller chercher
    //son poids sortant A(S)
    
    //découpes principales (transversales + verticales, pas de ligne)
    for(int ofsx = i1; ofsx < i2; ofsx++){ //de l'arête après i1 à l'arête devant i2
        int x = ofsx; //l'indice de l'arête supérieure
        int k;  //nombre d'arête centrale coupée, borné par delta
        for(int ofsz = j1; (k=abs(ofsx-ofsz) <= delta) && ofsz < j2; ofsz ++){
            int z = size-1 + ofsz; //l'indice de l'arête inférieure
            int ofsy = min(ofsx, ofsz); //ofset de la première arête centrale

            //A[size*(s-1) + x.first] *= wx //(soit G(x.first*size + x.second)) soit un tableau des poids
            //A[size*(s-1) + x.second] *= wx
            //A[size*(s-1) + z.first] *= wz
            //A[size*(s-1) + z.second] *= wz
            //Cost cost = A[size*(s-1) + size] * wx * wz

            for(int y = ofsy+D; y < ofsy+D+k; y++){
                //A[size*(s-1) + y.first] *= wy
                //A[size*(s-1) + y.second] *= wy
            }
            
        }
    }

    //coupes horizontales (les lignes)
    //avec 1 seule boucle on peut considérer 4 cas d'un coup (puisque double symétrie)
    for(int ofsx = i1; ofsx < i2; ofsx++){
        //cas ligne du haut
        //cas ligne du bas
        //cas ligne du haut en partant de i2
        //cas ligne du bas en partant de i2
        //+bucket mais.......
    }
}

void SS::init(string file){
    G.clear();
    A.clear();
    C.clear();
    E.clear();
    get_approx_solution(bestCost, bestOrder, file);

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                D = size/2;
                D2 = D*D;
                D3 = D*D*D;
                G.resize(size*(size+1), 1);
                C.resize(D3*D, -1);
            break;
            case 'e':
                flux >> i >> j >> w;
                E.push_back(make_pair(i,j));
                G[size*i + j] = w;
                G[size*j + i] = w;
            break;
            default:
            break;
        }
    }

    sort_edges(E);
}

void SS::execfile(string file){
    string path = "../instances/" + file;
    init(path);
    auto start = std::chrono::high_resolution_clock::now();
    bestCost = solve(0, size/2+1, 0, size/2+1);
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    cout << "Best cost : " << bestCost << '\n';
    cout << "Best order : ";
    display_order();
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void SS::execdir(string dir){
    string base = "../instances/" + dir + "/";
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(base.c_str());
    
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            string path = base + file->d_name;
            display(path.c_str()); //On verra plus tard pour modifier display() et les autres méthodes
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}