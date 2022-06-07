#include "Matrice/MatrixSolver.hpp"
#include "VerticalSplit/VerticalSplitSolver.hpp"
#include "SideIter/SideExSolver.hpp"
#include "Split/SplitSolver.hpp"
#include "TriScore/TriScore.hpp"
#include "TriScoreMargin/TriScoreM.hpp"
#include "TriScoreNaive/TriScoreN.hpp"
#include "SimpleGreedy/SimpleG.hpp"

MatrixSolver matrix;
SplitRange splitrange;
SimpleG simpleg;
SideIter sideiter;
Split split;
TriScore triscore;
TriScoreM mts;
NTS nts;
ofstream result_file("../results/results.txt");

void export_display(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    string filename(path);
    //cout << file << '\n';
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        exit(-1);
    }

    char mot[200];
    int n = 0;
    int nL = 0;
    while(getline(input_file, line) && line[0] != 'e'){
        if(line.size() > 2){
            istringstream flux(&line[2]);
            switch(line[0]){
                case 'v':
                   while(flux >> mot){
                        if(mot[0] == '*'){
                            char* padding;
                            if(nL == 1){
                                padding = "       ";
                            }else{
                                padding = "   ";
                            }
                            result_file << mot << "(" << n << ")" << padding;
                            n++;
                        }else{
                            result_file << mot << "   ";
                        }
                    }
                    nL++;
                    result_file << '\n';
                break;
                case 'p':
                    result_file << '\n' << "Sommets : " << &line[2] << '\n';
                break;
                default:
                break;
            }
        }
    }
    result_file << '\n';
}

void export_order(Tab O){
    result_file << "Best order : ";
    for(int i : O){
        result_file << i << " ";
    }
    result_file << '\n';
}

void export_matrix(const char* file){
    cout << "--- Matrix ---" << '\n';
    result_file << "--- Matrix ---" << '\n';
    matrix.execfile(file);
    result_file << "Best cost* : " << matrix.bestCost << '\n'; 
    result_file << "Time : " << matrix.time.count() <<endl;
}

void export_split(const char* file){
    cout << "--- Split ---" << '\n'; 
    result_file << "--- Split ---" << '\n';
    split.execfile(file);
    result_file << "Best cost* : " << split.bestCost << '\n';
    result_file << "Time : " << split.time.count() << endl;
}

void export_simpleg(const char* file){
    cout << "--- SimpleGreedy ---" << '\n';
    result_file << "--- SimpleGreedy ---" << '\n';
    simpleg.execfile(file);
    result_file << "Best cost* : " << simpleg.bestCost << '\n';
    export_order(simpleg.bestOrder);
    result_file << "Time : " << simpleg.time.count() << endl;
}

void export_vsplit(const char* file){
    cout << "--- VSplit ---" << '\n';
    result_file << "--- VSplit ---" << '\n';
    splitrange.delta = 3;
    result_file << "Delta : " << splitrange.delta << '\n'; 
    splitrange.execfile(file);
    result_file << "Best cost : " << splitrange.bestCost << '\n';
    result_file << "Time : " << splitrange.time.count() << endl;
}

void export_sideiter(const char* file){
    cout << "--- SideIter ---" << '\n';
    result_file << "--- SideIter ---" << '\n';
    sideiter.execfile(file);
    result_file << "Best cost : " << sideiter.bestCost << '\n';
    export_order(sideiter.bestOrder);
    result_file << "Time : " << sideiter.time.count() << endl;
}

void export_triscore(const char* file){
    cout << "--- TriScore ---" << '\n';
    result_file << "--- TriScore ---" << '\n';
    triscore.execfile(file);
    result_file << "Best cost : " << triscore.bestCost << '\n';
    export_order(triscore.bestOrder);
    result_file << "Time : " << triscore.time.count() << endl;
}

void export_mts(const char* file){
    cout << "--- TriScoreMargin ---" << '\n';
    result_file << "--- TriScoreMargin ---" << '\n';
    mts.execfile(file);
    result_file << "Best cost : " << mts.bestCost << '\n';
    export_order(mts.bestOrder);
    result_file << "Time : " << mts.time.count() << endl;
}

void export_nts(const char* file){
    cout << "--- TriScoreNaive ---" << '\n';
    result_file << "--- TriScoreNaive ---" << '\n';
    nts.execfile(file);
    result_file << "Best cost : " << nts.bestCost << '\n';
    export_order(nts.bestOrder);
    result_file << "Time : " << nts.time.count() << endl;
}

void execfile(const char* file){
    result_file << file << '\n';
    display(file);
    export_display(file);

    //export_matrix(file);

    //export_split(file);
    
    export_simpleg(file);
    
    export_vsplit(file);

    export_sideiter(file);

    export_triscore(file);   

    export_mts(file);

    export_nts(file);
    result_file << "----------" << "\n\n";
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