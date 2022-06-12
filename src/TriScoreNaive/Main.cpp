#include "TriScoreN.hpp"

int main(int argc, char* argv[]){
    NTS solver;
    switch(argc){
        case 1:
            cerr << "Missing argument : instance file" << endl;
            exit(-1);
            break;
        case 2:
            display(argv[1]);
            solver.execfile(argv[1]);
            break;
        case 3:
            if(argv[1] == string("d")){
                solver.execdir(argv[2]);
            }
            break;
        default:
            cerr << "Call error : wrong argument" << endl;
            exit(-1);
            break;
    }
    return 0;
}