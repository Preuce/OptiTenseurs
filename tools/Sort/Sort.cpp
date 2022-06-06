#include "Sort.hpp"

void sort_edges(std::vector<std::pair<int, int>>& E){
    int size = (2*E.size() + 4)/3;
    
    sort(E.begin(), E.end(), [size](std::pair<int, int> a, std::pair<int, int> b){
        if(a.first < b.first && a.second < b.second){
            return true;
        }else{
            if(a.first < size/2 && b.first < size/2){
                return a.second < b.second;
            }else{
                return a.first < size/2 && b.first >= size/2;
            }
        }
    });
}