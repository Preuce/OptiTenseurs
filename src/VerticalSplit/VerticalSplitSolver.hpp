#ifndef RANGESPLITSOLVER_HPP
#define RANGESPLITSOLVER_HPP
#include "../Split/SplitSolver.hpp"

/**
 * @brief Heuristique découpant le TT verticalement en tranche de taille max DELTA, et résolvant chaque sous-TT de manière exacte
 * 
 */

class SplitRange : public Algorithm{
    public:
    Tab G; //la matrice d'adjacence + 1 colonne donnant le poids sortant de chaque sommet
    Tab S; //un état (ensemble de sommets)
    Tab A; //un tableau donnant pour chaque taille d'état le poids sortant des sommets
    unordered_map<unsigned long long, Cost> C;//vector<Cost> C; //la liste des coûts obtenus
    unordered_map<unsigned long long, int> P1; //vector<long int> P1; //la liste des ordres S1
    unordered_map<unsigned long long, int> P2;//vector<long int> P2; //la liste des ordres S2

    Split solverGreedy;

    //A : Un tableau de taille n*n, le poids des arrêtes sortantes de chaque sommet, pour les n états S d'un "plongeon"
    //G : un tableau de taille n+1*n, la matrice d'adjacence + la colonne A de départ. M[i][j] = G[i*size+j]
    //S : Une liste de sommet, les sommets sélectionné pour cet état
    //C : une map associant au code binaire de chaque état un cout
    //P : une map associant au code binaire de chaque état un ordre

    //initialiseurs
    void init(string file); //initialise G, A, et S (S est simplement la liste des sommets au départ)
    Cost solve(Tab S); //calcule le coût
    Cost call_solve();

    //Renvoie A mis à jour pour le S actuel
    Tab computeA(Tab S);

    //renvoie le coût associé aux arètes liant directement S1 et S2 (les arètes non sortantes)
    Cost cut(Tab S1, Tab S2);
    //renvoie le coût sortant de S
    Cost produit_sortant(Tab S, Tab A);

    //converti un ensemble de sommets en un entier pouvant être stocké dans une map
    long int convert(Tab S);
    //converti une clé en l'ensemble de sommets correspondant
    Tab recover(unsigned long long key);
    Tab recover_full(Tab S);

    void display_order();
    void display_order(Tab S);
};

#endif