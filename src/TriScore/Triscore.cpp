#include "TriScore.hpp"
#include <algorithm>
#include <math.h>
#include <iostream>
#include <string.h>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>

/**
 * @brief solves G
 * 
 * @return int 
 */
int solve(){
    //gen et sort R
    sortRatio();

    int cost = 0;
    //tant qu'il reste des arètes
    while(!R.empty()){
        int i = R.front().first;
        //ajoute le coût du retrait de R.front à cost
        cost += ext_cost(i)*G[size*size + i];
        order.push_back(i);
        //R.popfront
        R.erase(R.begin());

        //update de G
        updateG(i);

        //update de R
        updateRatio();
    }
    return cost;
}

/**
 * @brief fills and sorts R, the list of scores
 * score of edge i = i's weight/ext_cost(i)
 */
void sortRatio(){
    for(int i = 0; i < size; i++){
        R[i] = (make_pair(i, G[size*(size) + i]/(float) ext_cost(i)));
    }
    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

/**
 * @brief mostly equivalent to sortRatio, updates the sorted list of scores R
 * It is sub-optimal to sort the entire list, especially considering that we have 3-4 edges to move at best
 */
void updateRatio(){
    for(auto& p : R){
        p.second = G[size*(size) + p.first]/(float) ext_cost(p.first);
    }
    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

/**
 * @brief computes the external cost associated an edge (doesn't account for the edge's weight itself)
 * 
 * @param i the Id of an edge
 * @return int 
 */
int ext_cost(int i){
    int res = 1;
    /*for(int k = 0; k < size; k++){
        res *= min(1, G[size*i + k]+1)*G[size*size + k];
    }*/
    for(int k = 0; k < size; k++){
        res *= max(1, G[size*i + k]*G[size*size + k]);
    }
    return res;
}

/**
 * @brief updates G given an edge to contract onto
 * 
 * @param i the Id of an edge
 */
void updateG(int i){
    Tab changed;
    //for(int j = 0; j < size; j++){
    for(auto& p : R){
        int j = p.first;
        //pour toutes les voisines de l'arète qu'on enlève
        if(G[size*i + j] > 0){
            //on supprime la liaison avec i
            G[size*j + i] = 0;
            G[size*i + j] = 0;
            //on stocke la voisine
            changed.push_back(j);
            int key = 0;
            for(auto& p : R){
                int k = p.first;
                //G[size*j + k] += G[size*i + k];
                G[size*j + k] = max(G[size*j + k], G[size*i + k]);
                //maintient de la symétrie, à voire si on peut s'en passer mais je crois pas
                G[size*k + j] = G[size*j + k];
                if(G[size*j + k] > 0){
                    key += pow(2, k);
                }
            }
            F[j] = key;
        }
    }

    for(int j = 0; j < changed.size(); j++){
        for(int k = j+1; k < changed.size(); k++){
            if(compare(changed[j], changed[k]) /*&& G[size*changed[j] + changed[k]] > 1*/){
                merge(changed[j], changed[k]);
            }
        }
    }
}

/**
 * @brief determines if 2 edges are equivalent based on their neighbors
 * 
 * @param i the Id of an edge
 * @param j the Id of an edge
 * @return true if the keys of the 2 without each other's link are equals
 * @return false 
 */
bool compare(int i, int j){ 
    return F[i]-pow(2, j) == F[j] - pow(2, i);
}

/**
 * @brief merges 2 equivalent edges into 1
 * 
 * @param i the Id of an edge
 * @param j the Id of an edge
 */
void merge(int i, int j){
    G[size*size + i] *= G[size*size + j];
    G[size*size + j] = 1;
}

/**
 * @brief displays the adjacence matrix G
 * 
 */
void displayG(){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
        cout << "|" << G[size*i + j] << "|";
        }
        cout << endl;
    }
}

void displayOrder(){
    cout << "| ";
    for(int i : order){
        cout << i << " | ";
    }
    cout << endl;
}

void get_size(char* Preamble){
	char c;
	char * pp = Preamble;
	int stop = 0;
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
	nbS = nbT;
    size = 3*(nbT/2) - 2;
}

void init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    R.clear();
    F.clear();
    order.clear();

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

    G.resize(size*(size+1), 0);
    F.resize(size);
    R.resize(size);
    
    Tab E (size);
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                if(i < nbS/2 && j < nbS/2){
                    E[min(i, j)] = w;
                }else if(i >= nbS/2 && j >= nbS/2){
                    E[min(i, j) + nbS/2 - 1] = w;
                }else{
                    E[max(i, j) - 1] = w;
                }
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    rempli(E);

    fclose(fp);
	delete[] Preamble;
}

//fills G given a list of weights
void rempli(Tab E){
    int ts1 = (size-1)/3;
    int ts2 = (size+2)/3;
    Tab Q1 (ts1);
    Tab N (ts2);

    R.clear();
    G.clear();
    F.clear();
    R.resize(size);
    G.resize(size*(size+1));
    F.resize(size);

    for(int i = 0; i < Q1.size(); i++){
        G[size*size + i] = E[i];
        G[size*size + size-1-i] = E[size-1-i];
        if(i != Q1.size() - 1){        
            G[size*i + i+1] = 1;
            G[size*(i+1) + i] = 1;

            G[size*(size-1-i) + size-2-i] = 1;
            G[size*(size-2-i) + size-1-i] = 1;
        }
        G[size*i + Q1.size() + i] = 1;
        G[size*i + Q1.size() + i+1] = 1;

        G[size*(Q1.size()+i) + i] = 1;
        G[size*(Q1.size()+i+1) + i] = 1;

        G[size*(size-1-i) + (size-1-i)-Q1.size()] = 1;
        G[size*(size-1-i) + (size-1-i)-Q1.size()-1] = 1;

        G[size*((size-1-i)-Q1.size()) + size-1-i] = 1;
        G[size*((size-1-i)-Q1.size()-1) + size-1-i] = 1;
    }

    for(int i = Q1.size(); i < Q1.size()+N.size(); i++){
        G[size*size + i] = E[i];
    }
}

void execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    cout << "Starting initialisation on : " << file << endl;
    init(path);
    displayG();
    cout << "End of initialisation" << endl;
    cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    int c = solve();
    cout << "Best cost : " << c << endl;
    cout << "Best order : ";
    displayOrder();

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
    switch(argc){
        case 1:
            cout << "Missing argument : instance file" << endl;
            return -1;
            break;
        case 2:
            execfile(argv[1]);
            break;
        case 3:
            if(*argv[1] == 'd'){
                execdir(argv[2]);
            }
            break;
        default:
            cout << "Call error : wrong argument" << endl;
            return -1;
            break;
    }
    return 0;
}