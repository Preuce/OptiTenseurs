#include "RangeSplitSolver.hpp"

int main(int argc, char* argv[]){
    SplitRange solver;
    const char* path;
    bool sf = true;

    if(argc == 1){
        cout << "Missing argument : instance file" << endl;
    }else{
        for(int i = 1; i < argc; i++){
            if(*argv[i] == 'd'){
                path = argv[i+1];
                sf = false;
                i++;
            }else if(*argv[i] == 'r'){
                solver.delta = atoi(argv[i+1]);
                i++;
            }else{
                path = argv[i];
            }
        }   
    }
    if(sf){
        display(path);
        solver.execfile(path);
    }else{
        solver.execdir(path);
    }
    return 0;
}