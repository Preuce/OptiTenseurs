#include "AdjaList.hpp"
#include <algorithm> 
#include <iostream>
#include <tuple>

/**
 * @brief contracts 2 tensors in a Tensor Network, adds it to the list of Contraction (ordre) and returns the associated contraction cost
 * 
 * @param t1 a Node
 * @param t2 a Node
 * @return the cost of contraction
 */
pair<int, Network> TensorNetwork::contract(Id e, Network N){
    Id s = edges[e].t.second; //le sommet a contracter
    int cost = edges[e].w;
    for(Id v : voisinage[s]){
        if(edges[v].t.first == s){
            edges[v].t.first = s;
        }else{
            edges[v].t.second = s;
        }
    }

    for(Id i : edges){
        if(edges[i].)
    }
}

void TensorNetwork::get_params(char* Preamble){

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
	tensors.resize(nbT);
}

void TensorNetwork::init(char* file){
    tensors.clear();
    validEdges.clear();
    //hardcoder les poids voisinant les sommets dans un fichier texte
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

    get_params(Preamble);

	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                {
                    tensors[i].push_back(make_pair(j, w));
                    tensors[j].push_back(make_pair(i, w));
                }
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    fclose(fp);
	delete[] Preamble;

    validEdges.resize(tensors.size(), true);
}