#include "EdgeSplit.hpp"

Cost ESplit::solve(int i1, int i2, int j1, int j2, CostTab A){
    unsigned long long key = convert(i1, i2, j1, j2);
    if(M.find(key) == M.end()){
        //TODO: stocker ext_cost de A dans AS[S.size()] ou quelque chose comme ça, de sorte que solve_diag et solve_vert puissent y accéder
        Cost S_cost;
        ext_cost(i1, i2, j1, j2, A, S_cost);
        //affiche(i1, i2, j1, j2);
        //affiche_A(A);
        //valeur par défaut, facilite les comparaisons
        M[key] = bestCost+1;
        //coupes diagonales
        solve_diag(i1, i2, j1, j2, key, A, S_cost);
        //coupes verticales
        solve_vertical(i1, i2, j1, j2,key, A, S_cost);
        
        //M[key] = 1;
    }
    return M[key];
}

Cost ESplit::solve_diag(int i1, int i2, int j1, int j2, unsigned long long key, CostTab A, Cost S_cost){
    //les blocs sont un peu difficiles à gérer, mais n'ont pas de découpe illégale en soit
    //il faut borner le sous-bloc pour éviter de couper les branches
    //les branches sont à gérer à part, elles ne doivent être coupées que verticalement
    int start = max(i1, j1); //le côté gauche du sous-bloc
    int end = min(i2, j2); //le côté droit du sous-bloc

    int ignored = E.size()-1;
    
    //on itère sur les arêtes centrales du sous-bloc
    //Si la forme est une ligne, on aura start > end et cette boucle sera passée 
    for(int ofsy = start; ofsy <= end; ofsy++){
        CostTab A_copy (A);
        //on multiplie de manière permanente le poids des extrémités de ofs (même fonctionnement que pour y_cost)
        //on multiplie de manière temporaire le poids des extrémités de start et de end (qu'on reset après chaque solve)
        int y = D-1+ofsy; //l'indice de l'arête
        //arête supérieure avant y, et arête inférieure avant y, initialisé à une valeur qui ne posera pas de problème
        int cx = y-D;
        int cz = y+D-1;
        //on vérifie que les arêtes sont dans le bloc, en comparant avec start
        //si elles le sont, on les prend en compte dans le coût de séparation

        //upward left piece
        int ula1=i1, ula2 /**/, ulb1=j1, ulb2=ofsy-1;
        //upward right piece
        int ura1, ura2=i2, urb1=j2 /**/, urb2=j2;
        //downward left piece
        int dla1=i1, dla2=ofsy-1, dlb1=j1, dlb2 /**/;
        //downward right piece
        int dra1=i2 /**/, dra2=i2, drb1, drb2=j2;

        //on ne les considère que si elles sont légales
        if(ofsy == start){
            ulb1 = -1;
            ulb2 = -1;
            dla1 = -1;
            dla2 = -1;

            dra1 = i1;
            urb1 = j1;
            //vraies valeurs de coût et d'arête
            cx = ignored;
            cz = ignored;
        }
        Cost start_x_cost = W.at(cx), start_z_cost = W.at(cz);

        //variable dans laquelle on va accumuler le produit des arêtes centrales
        Cost y_cost = 1;
        //on itère à nouveau sur les arêtes centrales, en se limitant aux DELTA-1 suivantes
        for(int ofs = ofsy; (ofs-ofsy+1 <= delta) && ofs <= end; ofs ++){
            
            ura1 = ofs+1;
            drb1 = ofs+1;
            
            ula2 = ofs;
            urb1 = min(urb1, ofs);
            dlb2 = ofs;
            dra1 = min(dra1, ofs);

            //dernière arête centrale
            int last_y = D-1+ofs;
            //arête finale en haut
            int x = ofs;
            //arête finale en bas
            int z = size-1 + ofs;
            //mise à jour du coût de séparation
            y_cost *= W.at(last_y);
            //mise à jour de A_copy
            A_copy.at(E[last_y].first) *= W[last_y];
            A_copy.at(E[last_y].second) *= W[last_y];

            Cost end_x_cost = W.at(x), end_z_cost = W.at(z);
            //on ne les considères que si elles sont légales
            if(ofs==end){
                ura1 = -1;
                ula2 = i2;

                ura2 = -1;
                drb1 = -1;
                drb2 = -1;
                dlb2 = j2;

                x = ignored;
                z = ignored;
                end_x_cost = 1;
                end_z_cost = 1;
            }

            //dans les 4 cas, on vérifie que le solving est inférieur au meilleur coût
            //si ce n'est pas le cas, on set le coût à bestCost et on ne solve pas le deuxième ensemble
            //si on a rien trouvé de concluant à la fin de l'exécution, on set M[key] à bestCost (ou bestCost+1 pour être sûr)
            {//première coupe : '/'
                //coût de séparation
                Cost cut = start_z_cost * y_cost * end_x_cost;
                Cost total = cut * S_cost;
                //on défini de nouvelles bornes i1, i2, j1, j2
                {//partie gauche
                    //on met à jour les poids sortant, si les coupures sont illégales, leur coût est à 1 -> pas de modification
                    A_copy.at(E.at(cz).first) *= start_z_cost;
                    A_copy.at(E.at(x).first) *= end_x_cost;
                    A_copy.at(E.at(cz).second) *= start_z_cost;
                    A_copy.at(E.at(x).second) *= end_x_cost;

                    total += solve(ula1, ula2, ulb1, ulb2, A_copy);
                }
            
                //on ne rentre que si le coût jusqu'à présent est inférieur au meilleur coût trouvé pour cet ensemble
                //if(total + cut < M[key])
                {//partie droite 
                    total += solve(ura1, ura2, urb1, urb2, A_copy);
                    if(total < M[key] && total > 0){
                        M[key] = total;
                        /*if(key==0x0000100010001){
                            cout << "Nouvelle valeur : " << M[key] << "\n\n";
                            cout << "Donnée par : " << '\n' << hex << convert(ula1, ula2, ulb1, ulb2) << '\n' << convert(ura1, ura2, urb1, urb2) << "\n\n";
                        }*/
                    }
                }
            }

            //on remet A_copy à un état antérieur
            A_copy.at(E.at(cz).first) /= W.at(cz);
            A_copy.at(E.at(cz).second) /= W.at(cz);
            A_copy.at(E.at(x).first) /= W.at(x);
            A_copy.at(E.at(x).second) /= W.at(x);

            {//deuxième coupe : '\'
            //coût de séparation
            Cost cut = start_x_cost * y_cost * end_z_cost;
            Cost total = cut * S_cost;
            //on défini de nouvelles bornes i1, i2, j1, j2
                {//partie gauche
                    A_copy.at(E.at(cx).first) *= start_x_cost;
                    A_copy.at(E.at(z).first) *= end_z_cost;
                    A_copy.at(E.at(cx).second) *= start_x_cost;
                    A_copy.at(E.at(z).second) *= end_z_cost;

                    total += solve(dla1, dla2, dlb1, dlb2, A_copy);
                }

                //on ne rentre que si le coût jusqu'à présent est inférieur au meilleur coût trouvé pour cet ensemble
                //if(total + cut < M[key])
                {//partie droite
                    /*if(convert(dra1, dra2, drb1, drb2) == 0x1FFFFFFFF){
                        cout << "Total1 : " << total << "\n";
                    }*/
                    total += solve(dra1, dra2, drb1, drb2, A_copy);
                    /*if(convert(dra1, dra2, drb1, drb2) == 0x1FFFFFFFF){
                        cout << "Total2 : " << total << "\n\n";
                    }*/

                    //TODO: condition sur le coût et mémoïsation
                    if(total < M[key] && total > 0){
                        M[key] = total;
                        /*if(key==0x0000100010001){
                            cout << "Nouvelle valeur : " << M[key] << "\n";
                            cout << "Donnée par : " << '\n' << hex << convert(dla1, dla2, dlb1, dlb2) << '\n' << convert(dra1, dra2, drb1, drb2) << dec << "\n\n";
                        }*/
                    }
                }
            }
        }   
    }
    return M[key];
}

void affiche(int i1, int i2, int j1, int j2){
    cout << i1 << " " << i2 << '\n' << j1 << " " << j2 << "\n\n";
}

void affiche_A(Tab A){
    for(int i = 0; i < A.size(); i++){
        cout << i << " : " << A[i] << '\n';
    }
    cout << '\n';
}

Cost ESplit::solve_vertical(int i1, int i2, int j1, int j2, unsigned long long key, CostTab A, Cost S_cost){
    int start = min(i1, j1);
    //Si on a plus d'1 sommet
    if((start >= 0) || i1!=i2 || j1!=j2){
        int ignored = E.size()-1;
        //cout << i1 << " " << i2 << " " << j1 << " " << j2 << '\n';
        for(int i = start; i < max(i2, j2); i++){
            //il ne faut pas couper (multiplier par le coût) des arêtes inutiles dans le cas des branches
            //on doit vérifier sur quelle ligne se trouve l'unique arête, et agir en conséquence
            //arête du haut
            int x = ignored;
            //arête du bas
            int z = ignored;

            Cost cut = 1;

            //cas on ne dépasse pas de la ligne i1 i2
            if(i >= i1 && i < i2){
                x = i;
                cut *= W.at(x);
                A.at(E.at(x).first) *= W.at(x);
                A.at(E.at(x).second) *= W.at(x);
            }

            //cas on ne dépasse pas de la ligne j1 j2
            if(i >= j1 && i < j2){
                z = size-1+i;
                cut *= W.at(z);
                A.at(E.at(z).first) *= W.at(z);
                A.at(E.at(z).second) *= W.at(z);
            }

            Cost total = cut * S_cost;

            {//partie gauche
                int a1=i1, a2=min(i, i2), b1=j1, b2=min(i, j2);

                //cas pas de ligne haute
                //mettre a2 à -1 garanti de passer la boucle précédente puisque min(-1, b2)=-1 > max(-1, b1)
                //de même on garanti i >= -1 && i < -1 = false ce qui permet d'ignorer l'arête
                if(i < i1){
                    a1 = -1;
                    a2 = -1;
                }
                //cas pas de ligne basse
                else if(i < j1){
                    b1 = -1;
                    b2 = -1;
                }

                total += solve(a1, a2, b1, b2, A);
                
            }

            {//partie droite
                int a1=max(i+1, i1), a2=i2, b1=max(i+1, j1), b2=j2;

                //cas pas de ligne haute
                if(i >= i2){
                    a1 = -1;
                    a2 = -1;
                }
                //cas pas de de ligne basse
                else if(i >= j2){
                    b1 = -1;
                    b2 = -1;
                }

                total += solve(a1, a2, b1, b2, A);
            }

            if(total < M[key] && total > 0){
                M[key] = total;
            }

            //on remet A à l'état précédent
            A.at(E.at(x).first) /= W.at(x);
            A.at(E.at(x).second) /= W.at(x);
            A.at(E.at(z).first) /= W.at(z);
            A.at(E.at(z).second) /= W.at(z);
        }
    }else{
        //cout << i1 << " " << i2 << " " << j1 << " " << j2 << '\n';
        M[key] = 0;
    }
    return M[key];
}

void ESplit::ext_cost(int i1, int i2, int j1, int j2, CostTab A, Cost& s){
    s = 1;
    if(i1 >= 0){
        for(int i = i1; i <= i2; i++){
            s *= A[i];
        }
    }
    if(j1 >= 0){
        for(int i = j1; i <= j2; i++){
            s *= A[i+D];
        }
    }
}

unsigned long long ESplit::convert(int i1, int i2, int j1, int j2){
    unsigned long long res = 0;
    res |= (unsigned long long) i1 << 48;
    res |= ((unsigned long long) i2&0x0000FFFF) << 32;
    res |= ((unsigned long long) j1&0x0000FFFF) << 16;
    res |= (j2&0X0000FFFF);
    return res;
}

void ComputeA(int i1, int i2, int j1, int j2, Tab& A){

}

void ESplit::init(string file){
    Tab G;
    cout << "Clearing..." << '\n';
    cout << "A" << '\n';
    A.clear();
    //cout << "M" << '\n';
    M.clear();
    cout << "W" << '\n';
    W.clear();
    cout << "E" << '\n';
    E.clear();
    cout << "bestOrder" << '\n';
    bestOrder.clear();
    cout << "Everything's clear" << '\n';
    //get_approx_solution(bestCost, bestOrder, file);
    bestCost = numeric_limits<short>::max() - 1;
    M[0xFFFFFFFFFFFFFFFF] = bestCost;

    cout << "Starting import" << '\n';
    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                D = size/2;
                if(delta <= 0){
                    delta = D;
                }else{
                    delta = min(delta, D);
                }
                A.resize(size+1, 1);
                G.resize(size*size, 1);
            break;
            case 'e':
                flux >> i >> j >> w;
                E.push_back(make_pair(i,j));
                G[size*i + j] = w;
                //G[size*j + i] = w;
            break;
            default:
            break;
        }
    }
    cout << "G and E initialized" << '\n';
    cout << "Sorting E..." << '\n';
    sort_edges(E);
    for(auto& p : E){
        W.push_back(G[size*p.first + p.second]);
    }
    cout << "E sorted" << '\n';
    //petite manip pour éviter des segfault
    cout << "Filling for segfault" << '\n';
    E.push_back(make_pair(size, size));
    W.push_back(1);
    cout << "End of fill" << '\n';
}

void ESplit::execfile(string file){
    string path = "../instances/" + file;
    cout << "Start of initialisation" << '\n';
    init(path);
    cout << "Initialisation ended" << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Start of solving" << '\n';
    bestCost = solve(0, D-1, 0, D-1, CostTab (size+1, 1));
    cout << "Solving ended" << '\n';
    cout << "Cost : " << bestCost << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    std::cout << std::scientific << "Temps : " << time.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void ESplit::execdir(string dir){
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