#ifndef TEST_HPP
#define TEST_HPP
#include "../../src/Components.hpp"
#include <fstream>
#include <sstream>
//prend un fichier texte en entrée
//la première ligne indique un fichier instance
//les lignes suivantes sont des suites de nombres correspondant à des arêtes
//pour chaque ligne, on effectue les contractions dans l'ordre, on affiche le résultat, et on l'ajoute à la fin de la ligne
//on arrête de lire une ligne si on atteint fin de ligne ou ":"

char path_test[100];
char path_instance[100];

int size;
vector<pair<int, int>> E;

Tab G;
Tab O;

void import_instance(const char* file);
Cost follow_order(Tab O);

class Graph{
    public:
    Tab G;
    Tab V;

    Cost contract(int i);
    int C(int i);

    void init();
};

#endif