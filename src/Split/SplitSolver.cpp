#include "SplitSolver.hpp"
#include <math.h>
#include <algorithm>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

/**
 * @brief Solves a given state
 * 
 * @param S The tensors in this state
 * @return int the best cost for S
 */
int Split::solve(Tab S){
    int key = convert(S);

    if(C[key] == -1 && S.size() > 1){
        int cost;
        
        Tab S1;
        Tab S2;

        int cout_sortant = produit_sortant(S, computeA(S));
        
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
                if(cost < C[key] || C[key] == -1){
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
            A[size*(S.size()-1)+i] /= G[size*i + k]; //TODO: KAKA
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
int Split::cut(Tab S1, Tab S2){
    int res = 1;
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
 * @param A The external-weight of all tensors for each states
 * @return int 
 */
int Split::produit_sortant(Tab S, Tab A){
    int res = 1;
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
int Split::convert(Tab S){
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
Tab Split::recover(int key){
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

void Split::display_order(Tab S){//pas fonctionnel
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

void Split::get_size(char* Preamble){

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

void Split::init(const char* file){
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

    S.resize(size);
    //cout << "S : " << S.size() << endl;
    G.resize(size*(size+1), 1);
    //cout << "G : " << G.size() << endl;
    A.resize(size*size, 1);
    //cout << "A : " << A.size() << endl;
    C.resize(pow(2, size), -1);
    //cout << "C : " << C.size() << endl;
    P1.resize(pow(2, size), -1);
    P2.resize(pow(2,size), -1);
    //cout << "P : " << P.size() << endl;

    for(int k = 0; k < size; k ++){
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

void Split::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    cout << "Starting initialisation on : " << file << endl;
    init(path);
    cout << "End of initialisation" << endl;
    cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : ";
    cout << solve(S) << endl;
    display_order(S);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
}

void Split::execdir(const char* dir){
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
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}

