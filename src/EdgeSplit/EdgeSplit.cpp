#include "EdgeSplit.hpp"
#include <iomanip>

Cost ESplit::solve(int i1, int i2, int j1, int j2, Tab A){
    unsigned long long key = convert(i1, i2, j1, j2);
    if(M.find(key) == M.end()){
        //valeur par défaut, facilite les comparaisons
        M[key] = bestCost+1;
        //coupes diagonales
        solve_diag(i1, i2, j1, j2, key, A);
        //coupes verticales
        solve_vertical(i1, i2, j1, j2,key, A);
        
        //M[key] = 1;
    }
    return M[key];
}

Cost ESplit::solve_diag(int i1, int i2, int j1, int j2, unsigned long long key, Tab A){
    //les blocs sont un peu difficiles à gérer, mais n'ont pas de découpe illégale en soit
    //il faut borner le sous-bloc pour éviter de couper les branches
    //les branches sont à gérer à part, elles ne doivent être coupées que verticalement
    int start = max(i1, j1); //le côté gauche du sous-bloc
    int end = min(i2, j2); //le côté droit du sous-bloc
    
    //on itère sur les arêtes centrales du sous-bloc
    //Si la forme est une ligne, on aura start > end et cette boucle sera passée 
    for(int ofsy = start; ofsy <= end; ofsy++){
        Tab A_copy (A);
        //on multiplie de manière permanente les extr de ofs (même foctionnement que pour y_cost)
        //on multiplie de manière temporaire les extr de start et de end (qu'on reset après chaque solve)
        int y = D-1+ofsy; //l'indice de l'arête
        //arête supérieure avant y, et arête inférieure avant y
        int cx = y-D;
        int cz = y+D-1;
        //on vérifie que les arêtes sont dans le bloc, en comparant avec start
        //si elles le sont, on les prend en compte dans le coût de séparation
        Cost start_x_cost = 1, start_z_cost = 1;
        //on ne les considère que si elles sont légales
        if(ofsy > start){
            start_x_cost = W.at(cx);
            start_z_cost = W.at(cz);
        }
        //variable dans laquelle on va accumuler le produit des arêtes centrales
        Cost y_cost = 1;
        //on itère à nouveau sur les arêtes centrales, en se limitant aux DELTA-1 suivantes
        for(int ofs = ofsy; (ofs-ofsy+1 <= delta) && ofs <= end; ofs ++){
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

            Cost end_x_cost = 1, end_z_cost = 1;
            //on ne les considères que si elles sont légales
            if(ofs < end){
                end_x_cost = W.at(x);
                end_z_cost = W.at(z);
            }

            //dans les 4 cas, on vérifie que le solving est inférieur au meilleur coût
            //si ce n'est pas le cas, on set le coût à bestCost et on ne solve pas le deuxième ensemble
            //si on a rien trouvé de concluant à la fin de l'exécution, on set M[key] à bestCost (ou bestCost+1 pour être sûr)
            {//première coupe : '/'
                //coût de séparation
                Cost cut = start_z_cost * y_cost * end_x_cost;
                //TODO: multiplier cut par le poids sortant de S
                //TODO: calculer le poids sortant de S1 et S2
                Cost total = 0;
                //on défini de nouvelles bornes i1, i2, j1, j2
                {//partie gauche
                    int a1=i1, a2=ofs, b1=j1, b2=ofsy-1;
                    //cas pas de ligne basse
                    if(ofsy==start){
                        b1=-1;
                        b2=-1;
                    }
                    //on met à jour les poids sortant, si les coupures sont illégales, leur coût est à 1 -> pas de modification
                    //TODO: dans les cas illégaux, cx ou z sortiront de la range de E -> segfault
                    /*A_copy.at(E.at(cz).first) *= start_z_cost;
                    A_copy.at(E[x].first) *= end_x_cost;*/
                    total += solve(a1, a2, b1, b2, A_copy);
                }
            
                //on ne rentre que si le coût jusqu'à présent est inférieur au meilleur coût trouvé pour cet ensemble
                //if(total + cut < M[key]){//partie droite
                    int a1=ofs+1, a2=i2, b1=ofsy, b2=j2;
                    //cas pas de ligne haute
                    if(ofs==end){
                        a1=-1;
                        a2=-1;
                    }
                    //on met à jour les poids sortant, si les coupures sont illégales, leur coût est à 1 -> pas de modification
                    //TODO:
                    /*A_copy.at(E.at(cz).second) *= start_z_cost;
                    A_copy.at(E[x].second) *= end_x_cost;*/
                    total += cut + solve(a1, a2, b1, b2, A_copy);

                    //TODO: condition sur le coût et mémoïsation
                    if(total < M[key]){
                        M[key] = total;
                    }
                //}
            }

            //on remet A_copy à un état antérieur
            //TODO:
            /*A_copy[E[cz].first] = A[E[cz].first];
            A_copy[E[cz].second] = A[E[cz].second];
            A_copy[E[x].first] = A[E[x].first];
            A_copy[E[x].second] = A[E[x].second];*/

            {//deuxième coupe : '\'
            //coût de séparation
            Cost cut = start_x_cost * y_cost * end_z_cost;
            //TODO: multiplier cut par le poids sortant de S
            //TODO: calculer le poids sortant de S1 et S2
            Cost total = 0;
            //on défini de nouvelles bornes i1, i2, j1, j2
                {//partie gauche
                    int a1=i1, a2=ofsy-1, b1=j1, b2=ofs;
                    //cas pas de ligne haute
                    if(ofsy==start){
                        a1=-1;
                        a2=-1;
                    }
                    //TODO:
                    /*A_copy[E.at(cx).first] *= start_x_cost;
                    A_copy[E[z].first] *= end_z_cost;*/

                    total += solve(a1, a2, b1, b2, A_copy);
                }

                //on ne rentre que si le coût jusqu'à présent est inférieur au meilleur coût trouvé pour cet ensemble
                //if(total + cut < M[key]){//partie droite
                    //TODO: il se peut qu'il y ai un pb ici, mais tant que les coûts sont pas calculé c'est dur à dire...
                    int a1=ofsy, a2=i2, b1=ofs+1, b2=j2;
                    //cas pas de ligne basse
                    if(ofs==end){
                        b1=-1;
                        b2=-1;
                    }
                    //TODO:
                    /*A_copy[E.at(cx).second] *= start_x_cost;
                    A_copy[E[z].second] *= end_z_cost;*/

                    total += cut + solve(a1, a2, b1, b2, A_copy);

                    //TODO: condition sur le coût et mémoïsation
                    if(total < M[key]){
                        M[key] = total;
                    }
                //}
            }
        }   
    }
}

Cost ESplit::solve_vertical(int i1, int i2, int j1, int j2, unsigned long long key, Tab A){
    //TODO: gérer les modif de A
    int start = min(i1, j1);
    if((start >= 0) || i1!=i2 || j1!=j2){
        //cout << i1 << " " << i2 << " " << j1 << " " << j2 << '\n';
        multiple++;
        for(int i = start; i < max(i2, j2); i++){
            //il ne faut pas couper (multiplier par le coût) des arêtes inutiles dans le cas des branches
            //on doit vérifier sur quelle ligne se trouve l'unique arête, et agir en conséquence
            //arête du haut
            int x = i;
            //arête du bas
            int z = size-1 + i;

            Cost cut = 1;

            //cas on ne dépasse pas de la ligne i1 i2
            if(i >= i1 && i < i2){
                cut *= W.at(x);
            }

            //cas on ne dépasse pas de la ligne j1 j2
            if(i >= j1 && i < j2){
                cut *= W.at(z);
            }

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

                solve(a1, a2, b1, b2, A);
                
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

                solve(a1, a2, b1, b2, A);
            }        
        }
    }else{
        //TODO: dafuq ?????
        //cout << i1 << " " << i2 << " " << j1 << " " << j2 << '\n';
        M[key] = 0;
        single++;
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

void ESplit::init(string file){
    G.clear();
    A.clear();
    M.clear();
    W.clear();
    E.clear();
    get_approx_solution(bestCost, bestOrder, file);
    M[0xFFFFFFFFFFFFFFFF] = bestCost+1;

    ifstream ifile(file);
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream flux(&line[2]);
        switch(line[0]){
            case 'p':
                size = atoi(&line[2]);
                D = size/2;
                delta = D; //TODO: temporaire je crois
                A.resize(size, 1);
                G.resize(size*(size+1), 1);
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
    for(auto& p : E){
        W.push_back(G[size*p.first + p.second]);
        //cout << W.back() << '\n';
    }
}

void ESplit::execfile(string file){
    string path = "../instances/" + file;
    init(path);
    //delta = D-1;
    int i1 = 0;
    int i2 = D-1;
    int j1 = 0;
    int j2 = D-1;
    auto start = std::chrono::high_resolution_clock::now();
    solve(i1, i2, j1, j2, A);
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
    cout << "seuls : " << single << '\n';
    cout << "multiple : " << multiple << '\n';
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