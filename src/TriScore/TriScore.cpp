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
Cost TriScore::solve(){
    int cost = 0;
    //tant qu'il reste des arètes
    while(!R.empty()){
        //cout << R.size() << endl;
        //cout << "in" << endl;
        int i = C(E[R.front().first].first);
        int k = C(E[R.front().first].second);
        if(i != k){
            //cout << ext_cost(2, 5) << endl;
            //cout << G[size*1 + C(5)] << endl;
            //ajoute le coût du retrait de R.front à cost
            cost += ext_cost(i, k)*G[size*i + k];
            contract(i, k);
            order.push_back(make_pair(i, k));
            //cout << cost << endl;
        }
        //cout << "(" << i << ", " << k << ")" << " : " << cost << endl;
        R.erase(R.begin());

        //update de R
        updateRatio();
        //cout << "out" << endl;
        //cout << cost << endl;
        //cout << G[size*2 + 5] << endl;;
       
    }
    return cost;
}

/**
 * @brief mostly equivalent to sortRatio, updates the sorted list of scores R
 * It is sub-optimal to sort the entire list, especially considering that we have 3-4 edges to move at best
 */
void TriScore::updateRatio(){
    for(auto& p : R){
        int i = C(E[p.first].first);
        int k = C(E[p.first].second);
        p.second = G[size*i + k]/(float) ext_cost(i, k);
    }
    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});
}

Cost TriScore::ext_cost(int i, int k){ //TODO: je pense que c'est cassé
    int res = 1;
    for(int j = 0; j < size; j++){
        if(k != j){
            res *= max(1, G[size*i + j]);
        }
        if(i != j){ 
            res *= max(1, G[size*k + j]);
        }
    }

    return res;
}

/**
 * @brief contracts an edge by modifying G and V accordingly
 * 
 * @param i
 * @param k
 */
void TriScore::contract(int i, int k){ //TODO: je pense que c'est cassé
    for(int j = 0; j < size; j++){
        G[size*i + j] *= G[size*k + j];
        G[size*k + j] = 0;
        G[size*j + k] = 0;
        G[size*j + i] = G[size*i + j];
    }
    V[k] = i;
}

int TriScore::C(int i){
    if(V[i] == -1){
        return i;
    }else{
        return C(V[i]);
    }
}

void TriScore::display_order(){
    for(auto& p: order){
        cout << "|(" << p.first << ", " << p.second << ")| ";
    }
    cout << endl;
}

void TriScore::get_size(char* Preamble){
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
    size = nbT;
}

void TriScore::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    R.clear();
    V.clear();
    E.clear();
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

    G.resize(size*size, 1);
    V.resize(size, -1);

    //E.reserve();
    //R.reserve();
    
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                E.push_back(make_pair(i, j));
                G[size*i + j] = w;
                G[size*j + i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    for(int i = 0; i < size; i ++){
        G[size*i + i] = 0;
    }

    for(int i = 0; i < E.size(); i++){
        int j = E[i].first;
        int k = E[i].second;
        R.push_back(make_pair(i, G[size*j  + k]/ (float) ext_cost(j, k)));
        //cout << G[size*j + k] << endl;;
    }

    sort(R.begin(), R.end(), [](pair<int, double> a, pair<int, double> b){return a.second > b.second;});

    fclose(fp);
	delete[] Preamble;
}

//fills G given a list of weights
/*void TriScore::rempli(Tab E){
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
}*/

void TriScore::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    int c = solve();
    cout << "Best cost : " << c << endl;
    cout << "Best order : ";
    display_order();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "--------------" << endl;
}

void TriScore::execdir(const char* dir){
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
