#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>
#include <unordered_map>
#include <limits>
#include "../tools/Sort/Sort.hpp"
#include "../tools/Display/Display.hpp"

using namespace std;

typedef int Cost;
typedef vector<Cost> CostTab;
typedef vector<int> Tab;

/*class Algorithm{
    public:
    //int delta = -1;
    int size;

    Cost bestCost;
    Tab bestOrder;
    std::chrono::duration<double> time;

    void execfile(string file);
    void execdir(string dir);
};*/

#endif