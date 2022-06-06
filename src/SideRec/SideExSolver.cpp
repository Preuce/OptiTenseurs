#include "SideExSolver.hpp"

Cost SideRec::solve(int s, int i){
    int ofs = s*s + i;
    
    if(s < size/2 - 1 && C[ofs] == -1){
        //cout << ofs << endl;
        computeA(s);
        for(int x = 2; x >= 0; x--){
            int p;
            switch(x){
                case 2:
                    p = i+2;
                    break;
                default:
                    p = x;
                    break;
            }
            int cost = contract(x, s) + solve(s+1, p);
            //cout << solve(s+1, p) << endl;
            
            if(cost < C[ofs] && cost > 0 || C[ofs] == -1){
                C[ofs] = cost;
                //lier la solution actuelle à la précédente/la suivante
                P[ofs] = p;
                //pour chaque truc dans P tu dis qui est son prédécesseur
                //chaque truc dans C dit quel est le coût min à partir de lui-même
                O[ofs] = Z[s];
            }
        }
        restoreA(s);
    }else if(T[s] < C[ofs] && T[s] > 0 || C[ofs] == -1){
        //cout << ofs << endl;
        //contracter les 2 sommets
        C[ofs] = T[s]; //a priori, les positions 2 et 3 seront inoccuppées mais osef
        //lier la solution actuelle à la précédente/la suivante
        //P[ofs] = p;
    }

    //rendre valeur stockée pour cet état
    return C[ofs];
}

/**
 * @brief 
 * 
 * @param s 
 */
void SideRec::computeA(int s){
    int i = s;
    int ofs = size*s;
    
    A[ofs + i] = T[i]*G[size*i + i+1];
    A[ofs + i+size/2] = T[i]*G[size*(i+size/2) + i+1+size/2];
}

/**
 * @brief 
 * 
 * @param s 
 */
void SideRec::restoreA(int s){
    int ofs = size*s;

    A[ofs + s] = G[size*size + s];
    A[ofs + s+size/2] = G[size*size +s+size/2];
}

//TODO: might be errors
/**
 * @brief 
 * 
 * @param x 
 * @param s 
 * @return Cost 
 */
Cost SideRec::contract(int x, int s){
    computeA(s);
    int i = s;
    int ofs = size*s;
    Cost G1 = G[size*(i+size/2) + i+1+size/2];
    Cost G0 = G[size*i + i+1];

    Cost r12;
    Cost r21;

    Cost r02;
    Cost r20;
    int cost;
    switch(x){ //c'est là qu'on return le min dans les 2 ordres possibles
        //cas arète du dessus
        case 0:
            T[i+1] = G0*G[size*(i+1) + i+1+size/2];
            //1 - 2
            r12 = T[i]*A[ofs + i+1+size/2] + A[ofs + i]*A[ofs + i+1+size/2]/G1; //TODO: virer les division
            //2 - 1
            r21 = A[ofs + i]*G1 + A[ofs + i+1+size/2]*G0;
            if(r12 < r21){
                cost = r12;
                Z[i] = make_pair(1, 2);
            }else{
                cost = r21;
                Z[i] = make_pair(2, 1);
            }
            break;
        //cas arète du dessous
        case 1:
            T[i+1] = G1*G[size*(i+1) + i+1+size/2];
            //0 - 2
            r02 = T[i]*A[ofs + i+1] + A[ofs + i+size/2]*A[ofs + i+1]/G0; //TODO: virer les divisions
            //2 - 0
            r20 = A[ofs + i+size/2]*G0 + A[ofs + i+1]*G1;
            if(r02 < r20){
                cost = r02;
                Z[i] = make_pair(0, 2);
            }else{
                cost = r20;
                Z[i] = make_pair(2, 0);
            }
            break;
        //cas arète centrale
        case 2:
            T[i+1] = T[i]*G[size*(i+1) + i+1+size/2];
            cost = T[i]*(A[ofs + i+1] + A[ofs + i+1+size/2]);
            Z[i] = make_pair(0, 1);
            break;
        default:
            cost = 0;
            break;
    }
    if(s == -1){
    cout << T[i+1] << '\n';
    cout << cost << '\n';
    cout << "G1 : " << G1 << '\n';
    cout << "G0 : " << G0 << '\n';
    cout << "2 - 1 : " << r21 << '\n';
    cout << "1 - 2 : " << r12 << '\n';
    cout << "A : " << '\n';
    for(int i = ofs; i < ofs + size; i++){
        cout << i - ofs << " " << A[i] << '\n';
    }
    }

    //cout << T[i+1] << '\n';
    //cout << cost << '\n';
    restoreA(s);
    //cout << cost << endl;
    return cost;
}

/**
 * @brief 
 * 
 * @param s 
 * @param i 
 */
void SideRec::display_order(int s, int i){
    int ofs = s*s + i;
    switch(P[ofs]){
        case -1:
            break;
        case 2:
            cout << "|(" << O[ofs].first << ", " << O[ofs].second << ")|";
            display_order(s+1, i+2);
            break;
        default:
            cout << "|(" << O[ofs].first << ", " << O[ofs].second << ")|";
            display_order(s+1, P[ofs]);
            break;
    }
    cout << endl;
}

void SideRec::get_size(char* Preamble){

	char c;
	char * pp = Preamble;
	int stop = 0;
	//tmp = (char *)calloc(100, sizeof(char));
	int nbT;
	
	while (!stop && (c = *pp++) != '\0'){
		switch (c){
            case 'c':
                while ((c = *pp++) != '\n' && c != '\0');
                break;
                
            case 'p':
                sscanf(pp, "%d\n", &nbT);
                stop = 1;
                break;
                
            default:
                break;
        }
	}
	size = nbT;
}

void SideRec::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    A.clear();
    R.clear();
    Q.clear();
    C.clear();
    T.clear();
    P.clear();
    Z.clear();
    O.clear();

    int MAX_PREAMBLE = 4000;
	char* Preamble = new char [MAX_PREAMBLE];

    int c, oc;
	char* pp = Preamble;

    int i, j, w;
	FILE* fp;

    if((fp=fopen(file,"r"))==NULL ){ 
        printf("ERROR: Cannot open infile\n"); 
        exit(10); 
    }

    for(oc = '\0';(c = fgetc(fp)) != EOF && (oc != '\n' || c != 'e'); 
    oc = *pp++ = c);

    ungetc(c, fp); 
	*pp = '\0';

    get_size(Preamble);

    G.resize(size*(size+1), 1);
    
    A.resize(size*size/2, 1); //TODO: a voir si c'est nécessaire d'avoir un tableau aussi grand
    
    R.resize(size/2, -1);
    Q.resize(size/2, -1);

    C.resize((size*size)/4, -1); //nombre total de valeurs à calculer

    T.resize(size/2, 1); //1 t par état, on peut réécrire par dessus sans soucis après être "remonté"
    P.resize((size*size)/4, -1); //nombre total de valeurs à calculer
    Z.resize(size/2,{-1, -1});
    O.resize((size*size)/4, {-1, -1});

	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                G[size*i + j] = w;
                G[size*j + i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            G[size*size + i] *= G[size*j + i];
        }
    }

    T[0] = G[size/2];

    for(int i = 0; i < size; i++){
        for(int k = 0; k < size/2; k++){
            A[size*k + i] = G[size*size + i];
        }
    }

    fclose(fp);
	delete[] Preamble;
}

void SideRec::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    //auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : " << solve(0, 0) << '\n';
    
    /*C[1*1 + 2] = contract(2, 0);
    C[2*2 + 4] = contract(2, 1);
    C[3*3 + 6] = contract(2, 2);
    for(int i = 0; i < C.size(); i++){
        cout << C[i] << '\n';
    }*/
    //auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> tempsSeq = end-start;
    //display_order(0, 0);
    //std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << '\n';
    cout << "--------------" << endl;
}

void SideRec::execdir(const char* dir){
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