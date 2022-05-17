#ifndef TRISCORE_HPP
#define TRISCORE_HPP
#include <vector>

using namespace std;

typedef vector<int> Tab;

int nbS;
int size; //en nombre de sommets
Tab G; //une matrice d'adjacence,avec les poids à la fin
vector<pair<int, double>> R; //La liste des scores triés
Tab F;
Tab order;

//pb : lié un score à l'arète correspondante
// -> R : paire<Id, score>

//G : matrice d'adjacence, où chaque case G[size*i + j] contient le poids de j par rapport à i (wj si reliés, 0 si soi-même, 1 sinon)
//pb : mettre à jour G
//pour tout j tel que G[size*i + j] > 1, on multiplie la ligne G[size*i] avec la ligne G[size*j], G[size*j + i] passe à 1

int solve();
//Calcule et trie le score de chaque arète
void sortRatio(); //ou void ?
//met à jour la liste des score après suppression d'une arète
void updateRatio(); //ou void ?
//calcule le coût associé à une arète i
int ext_cost(int i);
//met à jour G considérant la suppression de l'arète i
void updateG(int i);

bool compare(int i, int j);

void merge(int i, int j);

void rempli(Tab E);
//a priori on fait qu'un parcours, donc on peut se permettre de modifier directement R et G,
//S est pas forcément très utile, à part en condition d'arrêt ?
//on veut éviter de recalculer tout G, simplement le mettre à jour (sans suppr de données pour éviter les problèmes)

#endif