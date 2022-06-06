#include "SplitSmart.hpp"

Cost SplitSmart::solve(Tab S){
    
    if(S.size() > 1){
        for(int i = 0; i < S.size()-1; i++){
            Tab S1 (i+1);
            Tab S2 (S.size()-i-1);
            for(int k = 0; k <= i; k++){
                S1[k] = S[k];
            }

            for(int k = i+1; k < S.size(); k++){
                S2[k-i-1] = S[k];
            }
            for(int k : S1){
                cout << k << " ";
            }
            cout << " | ";
            for(int k : S2){
                cout << k;
            }
            cout << '\n';

            //solve(S1), solve(S2)
        }
    }else{
        //cas 1 arête restante
    }
    return 0;
}



void SplitSmart::init(const char* file){

}

void SplitSmart::execfile(const char* file){
    Tab S = {0, 1, 2 ,3 ,4, 5};
    solve(S);
}

void SplitSmart::execdir(const char* dir){

}