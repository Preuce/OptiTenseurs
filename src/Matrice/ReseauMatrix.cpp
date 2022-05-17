#include "ReseauMatrix.hpp"
#include <stdlib.h>
#include <string.h>
#include <iostream>

/**
 * @brief contracts an edge in the matrix N and returns the newly formed matrix
 * 
 * @param i 
 * @param j 
 * @param N
 * @return Network
 */
Network contract(int j, int i, Network N){
    for(unsigned int k = 0; k < N.size(); k++){
        //migration des poids vers la j-ième ligne/colonne
        N[k][j] *= N[k][i]; //TODO: trouver une alternative à la matrice symétrique
        N[j][k] *= N[k][i];
    }
    for(unsigned int k = 0; k < N.size(); k++){
        N[k].erase(N[k].begin() + i);
    }
    //suppression de la i-ième colonne
    N.erase(N.begin() + i);
    return N;
}

/**
 * @brief calculates the contractions cost of the edge on (i, j) in the matrix N
 * 
 * @param i 
 * @param j 
 * @param N
 * @return int 
 */
int contractionCost(int i, int j, Network N){
    int cost = N[i][j];
    for(unsigned int k = 0; k < N.size(); k++){
        cost *= max(1, N[k][j]*N[k][i]);
    }
    return cost;
}

/**
 * @brief Displays a matrix
 * 
 * @param N 
 */
void display(Network N){
    for(unsigned int i = 0; i < N.size(); i++){
        for(unsigned int j = 0; j < N.size(); j++){
            cout << N[j][i] << " | ";
        }
        cout << endl;
    }
}

/**
 * @brief Displays the matrix representation of the Tensor network
 * 
 */
void MatrixNetwork::display(){
    for(unsigned int i = 0; i < size; i++){
        for(unsigned int j = 0; j < size; j++){
            cout << network[j][i] << " | ";
        }
        cout << endl;
    }
}

void MatrixNetwork::get_size(char* Preamble){

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

void MatrixNetwork::init(const char* file){
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

    get_size(Preamble);

    network.resize(size);

    for(int k = 0; k < size; k ++){
        network[k].resize(size, 1);
        network[k][k] = 0;
    }
	
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                network[i][j] = w;
                network[j][i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    fclose(fp);
	delete[] Preamble;
}