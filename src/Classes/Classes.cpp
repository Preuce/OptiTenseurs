#include "Classes.hpp"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h>

int contractionCost(Id i, Id j, Network N){
    int cost = 1;
    Classe c1 = N[i];
    Classe c2 = N[j];
    vector<pair<Id, Weight>> v = c1.voisins;
    //fusion des listes de voisins
    v.insert(v.end(), c2.voisins.begin(), c2.voisins.end());

    for(pair<Id, Weight> p : v){
        if(p.first != c1.sommets.front()){
            cost *=p.second;
        }
    }
    /*for(int k = 0; k < v.size(); k++){
        if(v[k].first != c1.sommets[0]){
            cost *= v[k].second;
        }
    }*/
    return cost;
}

Network contract(Id i, Id j, Network N){
    Classe* c1 = &N[i];
    Classe c2 = N[j]; //éventuellement passer par des pointeurs
    //fusion des listes de sommets
    c1->sommets.insert(c1->sommets.end(), c2.sommets.begin(), c2.sommets.end()); //TODO: passer en liste
    
    //fusion des listes de voisins
    c1->voisins.insert(c1->voisins.end(), c2.voisins.begin(), c2.voisins.end()); //TODO: passer en liste

    sort(c1->voisins.begin(), c1->voisins.end(), [](pair<Id, Weight> a, pair<Id, Weight> b){return a.first < b.first;});

    //pour chaque voisins de c2
    for(pair<Id, Weight> v1 : c2.voisins){
        //on récupère tous ses voisins
        vector<pair<Id, Weight>> v2 = N[v1.first].voisins;

        //pour chaque voisin
        for(int i = 0; i < v2.size(); i++){
            //si il est voisin de la classe qu'on enlève
            if(v2[i].first == c2.sommets.front()){
                //on remplace par la nouvelle classe
                N[v1.first].voisins[i].first = c1->sommets.front();
                break;
            }
        }
    }

    for(int i = 0; i < c1->voisins.size(); i++){
        //si on est lié 2 fois à la même classe
        if(i < c1->voisins.size() - 1 && c1->voisins[i].first == c1->voisins[i+1].first){
                c1->voisins[i].second *= c1->voisins[i+1].second;
                c1->voisins.erase(c1->voisins.begin() + i+1);
        }
        //si on est lié à nous-même
        if(c1->voisins[i].first == c1->sommets[0] || c1->voisins[i].first == c2.sommets[0]){
            c1->voisins.erase(c1->voisins.begin() + i);
            i--;
        }
    }

    //N.erase(N.begin() + j);
    return N;
}

void ClassesNetwork::get_size(char* Preamble){

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
	int size = nbT;
    matrice.resize(nbT*nbT, 1);
    for(unsigned int i = 0; i < nbT; i++){
        Classe c = {{i}, {}};
        network.push_back(c);
        matrice[nbT*i + i] = 0;
    }
}

void ClassesNetwork::init(const char* file){
    //TODO: faire les imports
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
	
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                network[i].voisins.push_back(make_pair(j, w));
                network[j].voisins.push_back(make_pair(i, w));
                matrice[network.size()*i+j] = w;
                matrice[network.size()*j+i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    fclose(fp);
	delete[] Preamble;
}