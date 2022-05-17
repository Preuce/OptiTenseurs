#ifndef RESEAUMATRIX_HPP
#define RESEAUMATRIX_HPP
#include <vector>

using namespace std;

typedef vector<vector<int>> Network;

Network contract(int i, int j, Network N);
int contractionCost(int i, int j, Network N);
void display(Network N);

class MatrixNetwork{
    public:
    int size;
    Network network;

    void display();
    void get_size(char* Preamble);
    void init(const char* file);
};
#endif