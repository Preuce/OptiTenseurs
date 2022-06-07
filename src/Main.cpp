#include "Matrice/MatrixSolver.hpp"
#include "VerticalSplit/VerticalSplitSolver.hpp"
//#include "SideRec/SideExSolver.hpp"
#include "SideIter/SideExSolver.hpp"
#include "Split/SplitSolver.hpp"
#include "TriScore/TriScore.hpp"
#include "SimpleGreedy/SimpleG.hpp"

MatrixSolver matrix;
SplitRange splitrange;
SimpleG simpleg;
SideIter sideiter;
Split split;
TriScore triscore;

void execfile(const char* file){
    display(file);
    
    //cout << "--- Matrix ---" << endl;
    //matrix.execfile(file);

    //cout << "--- Split ---" << '\n'; 
    //split.execfile(file);

    cout << "--- SimpleGreedy ---" << '\n';
    simpleg.execfile(file);

    cout << "--- SplitRange ---" << '\n';
    splitrange.delta = 3;
    splitrange.execfile(file);

    cout << "--- SideIter ---" << '\n';
    sideiter.execfile(file);

    cout << "--- TriScore ---" << '\n';
    triscore.execfile(file);
}

void execdir(const char* dir){
    char base[100] = "../instances/";
    strcat(base, dir);
    strcat(base, "/");
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(base);
    
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            char path[100];
            strcpy(path, base);
            strcat(path, file->d_name);

            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}

int main(int argc, char* argv[]){
    switch (argc){
    case 1:
        cout << "Missing argument : instance file" << endl;
        return -1;
        break;
    case 2:
        execfile(argv[1]);
        break;
    case 3:
        if(*argv[1] == 'd'){
            execdir(argv[2]);
        }else if(strcmp(argv[1], "vd")==0){
            display_dir(argv[2]);
        }else if(*argv[1] == 'v'){
            display(argv[2]);
        }/*else if(*argv[1] == 't'){
            //fonction de test sur argv[2]
        }*/

        break;
    default:
        cout << "Error : check arguments list" << endl;
        break;
    }
    return 0;
}