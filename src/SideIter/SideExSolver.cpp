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

void SideIter::get_size(char* Preamble){

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

void SideIter::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    A.clear();
    P.clear();

    C.clear();
    T.clear();
    O.clear();
    Z.clear();

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
    
    A.resize(size, 1);

    P.resize(size, -1);
    
    C.resize(2*(size/2)-1, 0); //tableau des coûts

    T.resize(2*(size/2)-1, 1); //tableau des t

    O.resize(size*size/4, {-1, -1});
    Z.resize(size/2, -1);

	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                G[size*i + j] = w;
                G[size*j + i] = w;
                G[size*size + i] *= w;
                G[size*size + j] *= w;
                A[i] *= w;
                A[j] *= w;

                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    fclose(fp);
	delete[] Preamble;
}

void SideIter::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : " << solve() << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    display_order(size/2-2, Z[size/2-1]);
    cout << '\n';
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << '\n';
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