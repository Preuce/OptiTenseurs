#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <vector>

using namespace std;

typedef unsigned int Id;
typedef unsigned int Weight;

//classe : représentant ?, sommets (entiers) et (classe - poids) 

//je stocke la liste des classes


//contracter 2 classes [a] et [b] : produit des poids de l'arrête * produits des poids de tous les voisins
//mettre à jour : 
//  tous les voisins qui pointaient vers [b], pointent vers [a]
//  les sommets de [b] sont ajoutés aux sommets de [a]
//      toutes les liaisons dans [a] vers [b] sont supprimées/ont un poids de 1
//      si une liaison apparaît plusieurs fois, on multiplie leurs poids et on les supprime toutes sauf une

//permutations :
//prendre une classe
//  prendre une liaison de poids > 1
//  contracter
//  continuer avec une classe en moins

struct Classe{
    vector<Id> sommets;
    vector<pair<Id, Weight>> voisins;
};

typedef vector<Classe> Network;

int contractionCost(Id i, Id j, Network N);
Network contract(Id i, Id j, Network N);

class ClassesNetwork{
    Network network;
    vector<Weight> matrice;
    
    public:
    Network getNetwork(){return network;}
    vector<Weight> getMatrice(){return matrice;}
    void get_size(char* Preamble);
    void init(const char* file);
};

#endif