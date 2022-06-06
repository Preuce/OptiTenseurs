#include "RangeSplitSolver.hpp"

/**
 * @brief Solves a given state
 * 
 * @param S The tensors in this state
 * @return int the best cost for S
 */
Cost SplitRange::solve(Tab S){
    long int key = convert(S);

    if(C[key] == -1 && S.size() > 1){
        Cost cost;
        
        Tab S1;
        Tab S2;

        Cost cout_sortant = produit_sortant(S, computeA(S));
        
        //pour toutes les séparations de S
        //do{
            //on déplace la "barre" où on fait la coupure S1/S2 dans S
            for(int i = 0; i < min(delta, (int) S.size()); i++){
                S1.clear();
                S2.clear();
                //on attribue les sommets de S1
                for(int k = 0; k <= i; k++){
                    S1.push_back(S[k]);
                    S1.push_back(S[k] + size/2);
                }
                //on attribue les sommets de S2
                for(int k = i+1; k < S.size(); k++){
                    S2.push_back(S[k]);
                }
                cost = solverGreedy.solve(S1) + solve(S2) + cout_sortant*cut(S1, S2);
                if(cost < C[key] && cost > 0|| C[key] == -1){
                    C[key] = cost;
                    P1[key] = convert(S1); //TODO: pas bon
                    P2[key] = convert(S2);
                }
            }
        //}while(next_permutation(S.begin(), S.end()));
    }else if(S.size() == 1){
        Tab p = {S[0], S[0] + size/2};
        C[key] = solverGreedy.solve(p);
    }
    return C[key];
}

/**
 * @brief computes the S.size()-1'th column of A
 * 
 * @param S The tensors in this state
 * @return Tab an updated copy of A
 */
Tab SplitRange::computeA(Tab S){
    for(int i : S){
        A[size*(S.size()-1) + i] = G[size*size + i];
        A[size*(S.size()-1) + i+size/2] = G[size*size + i+size/2];
        
        for(int k : S){
            A[size*(S.size()-1) + i] /= G[size*i + k]; //TODO: KAKA
            A[size*(S.size()-1) + i] /= G[size*i + k+size/2];
            A[size*(S.size()-1) + i + size/2] /= G[size*(i+size/2) + k];
            A[size*(S.size()-1) + i + size/2] /= G[size*(i+size/2) + k + size/2];
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
Cost SplitRange::cut(Tab S1, Tab S2){
    Cost res = 1;
    for(int i : S1){
        for(int j : S2){
            res *= G[size*i + j];
            res *= G[size*i + j + size/2];
        }
    }
    return res;
}

/**
 * @brief computes the product of all the edges leaving a state
 * 
 * @param S The tensors in this state
 * @param A The external-weight of all tensors for each states
 * @return int 
 */
Cost SplitRange::produit_sortant(Tab S, Tab A){
    Cost res = 1;
    for(int i : S){
        res *= A[size*(S.size()-1) + i];
        res *= A[size*(S.size()-1) + i + size/2];
    }
    return res;
}

/**
 * @brief converts a state in a unique integer key
 * 
 * @param S The tensors in this state
 * @return int 
 */
long int SplitRange::convert(Tab S){
    int res = 0;
    for(int i : S){
        if(i < size/2){
            res += pow(2, i);
        }
    }
    return res;
}

/**
 * @brief converts a key into a set of tensors
 * 
 * @param key a code generated from a state using convert(S)
 * @return Tab 
 */
Tab SplitRange::recover(long int key){ //TODO: repasser en "norme classique" pour display solverGreedy
    Tab res;
    for(int i = size/2; i >= 0; i--){
        int p = pow(2, i);
        if(key >= p){
            res.push_back(i);
            res.push_back(i+size/2);
            key -= p;
        }
    }
    return res;
}

Tab SplitRange::recover_full(Tab S){
    Tab res;
    for(int i : S){
        res.push_back(i);
        res.push_back(i+size/2);
    }
    return res;
}

void SplitRange::display_order(Tab S){
    if(S.size() >= 1){
        long int key = convert(S);
        if(key != -1){
            Tab p1K = recover(P1[key]);
            solverGreedy.display_order(p1K);
            if(!p1K.empty() && p1K.size() != S.size()){
                //display_order(recover(P1[key]));
                display_order(recover(P2[key]));
            }
            cout << "| ";
            for(int i : recover(key)){
                cout << i << " | ";
            }
            cout << endl;
        }
    }
}

void SplitRange::get_size(char* Preamble){
	char c;
	char * pp = Preamble;
	int stop = 0;
	//tmp = (char *)calloc(100, sizeof(char));
	int nbT;
	
	while(!stop && (c = *pp++) != '\0'){
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

void SplitRange::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    S.clear();
    G.clear();
    A.clear();
    C.clear();
    P1.clear();
    P2.clear();

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

    S.resize(size/2);
    //cout << "S : " << S.size() << endl;
    G.resize(size*(size+1), 1);
    //cout << "G : " << G.size() << endl;
    A.resize(size*size, 1);
    //cout << "A : " << A.size() << endl;
    C.resize(pow(2, size/2), -1);
    //cout << "C : " << C.size() << endl;
    P1.resize(pow(2, size/2), -1);
    P2.resize(pow(2,size/2), -1);
    //cout << "P : " << P.size() << endl;

    for(int k = 0; k < size/2; k ++){
        S[k] = k;
    }
	
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

    fclose(fp);
	delete[] Preamble;
}

void SplitRange::execfile(const char* file, int d){
    char path[100] = "../instances/";
    strcat(path, file);
    cout << "Starting initialisation on : " << file << endl;
    init(path);
    solverGreedy.init(path);
    delta = d;
    cout << "End of initialisation" << endl;
    cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : ";
    cout << solve(S) << endl;

    /*for(int i = 0; i < P1.size(); i++){
        cout << P1[i] << endl;
    }*/
    display_order(S);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
    delta = -1;
}

void SplitRange::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    solverGreedy.init(path);
    if(delta <= 0){
        delta = size/2;
        //cout << delta << endl;
    }
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    cout << "Range : " << delta << endl;
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : ";
    cout << solve(S) << endl;

    /*for(int i = 0; i < P1.size(); i++){
        cout << P1[i] << endl;
    }*/
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    display_order(S);
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
    delta = -1;
}

void SplitRange::execdir(const char* dir){
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
