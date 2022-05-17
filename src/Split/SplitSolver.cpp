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
 * @brief 
 * 
 * @param delta the horizontal size of a slice 
 * @return int 
 */
int Split::splitRange(int delta){
    int cost = INT32_MAX;
    vector<Tab> v;

    if(delta >= size/2){
        cost = solve(S);
    }else{
        //pour chaque décalage possible dans delta
        for(int d = 0; d <= (size/2)%delta; d++){
            int localcost = 0;
            for(int i = 0; i < v.size(); i++){
                v[i].clear();
            }
            v.clear();
            //int taille = size/(2*delta) + 2;
            v.resize(2 + size/(2*delta)); //TODO: pb here
            //même dans le cas d'un perfect fit on doit considérer qu'on va déplacer nos tranches
            //donc on a besoin de pouvoir gérer les "restes" à droite et à gauche

            //on s'intéresse aux premiers, qu'on a pas pu ranger
            for(int i = 0; i < d; i++){
                v[0].push_back(i);
                v[0].push_back(i+size/2);
            }

            //pour chaque i dans delta
            for(int i = 0; i < delta; i++){
                //pour chaque tranche delta
                for(int k = 0; k < size/(2*delta); k++){
                    v[k+min(1, d)].push_back(k*delta + i + d);
                    v[k+min(1, d)].push_back(k*delta + i + size/2 + d);
                }
            }
            
            //on s'intéresse aux derniers, qu'on a pas pu ranger
            for(int i = 0; i < (size/2)%delta - d/*(size/2 - d)%delta*/; i++){
                v[v.size()-1].push_back((v.size()-1)*delta + d +i);
                v[v.size()-1].push_back((v.size()-1)*delta + d + i + size/2);
            }

            //on solve pour chaque split non-vide
            for(Tab s : v){
                if(!s.empty()){
                    cout << s.size() << endl;
                    localcost += solve(s);
                }
            }
            //récupérer les résultats, en faire un nouveau réseau et faire la contraction des sous-sensemble entre eux
            Split newOrga = rearrange(v);
            localcost += newOrga.solve(newOrga.S);
            cost = min(cost, localcost);
        }    
    }
    return cost;
}

Split Split::rearrange(vector<Tab> v){
    Split s;
    s.size = v.size();
    s.S.clear();
    s.S.resize(s.size);

    s.G.clear();
    s.G.resize(s.size*(s.size+1), 1);

    s.A.clear();
    s.A.resize(s.size*s.size, 1);

    s.C.clear();
    s.C.resize(pow(2, s.size), -1);

    s.P1.clear();
    s.P1.resize(pow(2, s.size), -1);

    s.P2.clear();
    s.P2.resize(pow(2,s.size), -1);

    for(int i = 0; i < v.size(); i++){
        //S
        s.S[i] = i;

        //G partie adjacence
        if(i < v.size() - 1){
            int w = cut(v[i], v[i+1]);
            s.G[s.size*i + i+1] = w;
            s.G[s.size*(i+1) + i] = w;
        }
        //G partie poids sortant
        s.G[s.size*s.size + i] = produit_sortant(v[i], computeA(v[i]));
    }
    return s;
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

void execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    cout << "Starting initialisation on : " << file << endl;
    solver.init(path);
    if(range <= 0){
        range = solver.size/2;
        cout << range << endl;
    }
    cout << "End of initialisation" << endl;
    cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    cout << "Best cost : ";
    //cout << solver.solve(solver.S) << endl;
    cout << solver.splitRange(range) << endl;
    //solver.display_order(solver.S);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
}

void execdir(const char* dir){
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

int main(int argc, char* argv[]){

    if(argc == 1){
        cout << "Missing argument : instance file" << endl;
    }else{
        for(int i = 1; i < argc; i++){
            if(*argv[i] == 'd'){
                path = argv[i+1];
                sf = false;
                i++;
            }else if(*argv[i] == 'r'){
                range = atoi(argv[i+1]);
                i++;
            }else{
                path = argv[i];
            }
        }   
    }
    if(sf){
        execfile(path);
    }else{
        execdir(path);
    }
    return 0;
}