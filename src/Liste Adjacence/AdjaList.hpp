#ifndef ADJALIST_HPP
#define ADJALIST_HPP

#include <vector>
#include <map>
using namespace std;

typedef unsigned int Weight;
typedef unsigned int Id;

typedef vector<Edge> Network;

struct Edge{
    Weight w;
    pair<Id, Id> t;
};

class TensorNetwork{
    
private:
    Network edges; //les arètes
    map<Id, vector<Id>> voisinage; //les voisinages des sommets

protected:

public:
    //getters


    //contraction-related functions
    pair<int, Network> contract(Id e, Network N); //ref indéfinie vers intersection et symmdiff
    int contractionCost(Id t1, Id t2); //ref indéfinie vers intersection et symmdiff
    //
    void init(char* file);
    void get_params( char * Preamble);
};
#endif