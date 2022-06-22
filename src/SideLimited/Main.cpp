#include "SideLimSolver.hpp"

int main(int argc, char* argv[]){
    SLim solver;
    const char* path;
    bool sf = true;

    if(argc == 1){
        cerr << "Missing argument : instance file" << endl;
    }else{
        for(int i = 1; i < argc; i++){
            if(argv[i] == string("d")){
                if(i == argc-1){
                    cerr << "Missing argument : directory" << '\n';
                }
                path = argv[i+1];
                sf = false;
                i++;
            }else if(argv[i] == string("r")){
                if(i == argc-1){
                    cerr << "Missing argument : range" << '\n';
                }
                solver.refdelta = atoi(argv[i+1]);
                cout << "real delta" << solver.delta << '\n';
                i++;
            }else{
                path = argv[i];
            }
        }   
    }
    if(sf){
        display(path);
        execfile<SLim>(solver, path);
    }else{
        execdir<SLim>(solver, path);
    }
    return 0;
}