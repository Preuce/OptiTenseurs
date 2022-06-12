#include "Matrice/MatrixSolver.hpp"
#include "VerticalSplit/VerticalSplitSolver.hpp"
#include "SideIter/SideExSolver.hpp"
#include "SideLimited/SideLimSolver.hpp"
#include "Split/SplitSolver.hpp"
#include "TriScore/TriScore.hpp"
#include "TriScoreMargin/TriScoreM.hpp"
#include "TriScoreNaive/TriScoreN.hpp"
#include "SimpleGreedy/SimpleG.hpp"

MatrixSolver matrix;
SplitRange splitrange;
SimpleG simpleg;
SideIter sideiter;
SLim sidelim;
Split split;
TriScore triscore;
TriScoreM mts;
NTS nts;

ofstream result_file("../results/results.txt");
string instance_dir;
string instance_file;

void export_display(){
    string path = "../instances/" + instance_file;
    string filename(path);
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

void export_matrix(){
    cout << "--- Matrix ---" << '\n';
    result_file << "--- Matrix ---" << '\n';
    matrix.execfile(instance_file);
    result_file << "Best cost* : " << matrix.bestCost << '\n'; 
    result_file << "Time : " << matrix.time.count() <<endl;
}

void export_split(){
    cout << "--- Split ---" << '\n'; 
    result_file << "--- Split ---" << '\n';
    split.execfile(instance_file);
    result_file << "Best cost* : " << split.bestCost << '\n';
    result_file << "Time : " << split.time.count() << endl;
}

void export_simpleg(){
    cout << "--- SimpleGreedy ---" << '\n';
    result_file << "--- SimpleGreedy ---" << '\n';
    simpleg.execfile(instance_file);
    result_file << "Best cost* : " << simpleg.bestCost << '\n';
    export_order(simpleg.bestOrder);
    result_file << "Time : " << simpleg.time.count() << endl;
}

void export_vsplit(){
    cout << "--- VSplit ---" << '\n';
    result_file << "--- VSplit ---" << '\n';
    splitrange.delta = 3;
    result_file << "Delta : " << splitrange.delta << '\n'; 
    splitrange.execfile(instance_file);
    result_file << "Best cost : " << splitrange.bestCost << '\n';
    result_file << "Time : " << splitrange.time.count() << endl;
}

void export_sideiter(){
    cout << "--- SideIter ---" << '\n';
    result_file << "--- SideIter ---" << '\n';
    sideiter.execfile(instance_file);
    result_file << "Best cost : " << sideiter.bestCost << '\n';
    export_order(sideiter.bestOrder);
    result_file << "Time : " << sideiter.time.count() << endl;
}

void export_sidelim(){
    cout << "--- SideLimited ---" << '\n';
    result_file << "--- SideLimited ---" << '\n';
    sidelim.delta = 3;
    result_file << "Delta : " << sidelim.delta << '\n';
    sidelim.execfile(instance_file);
    result_file << "Best cost : " << sidelim.bestCost << '\n';
    export_order(sideiter.bestOrder);
    result_file << "Time : " << sidelim.time.count() << endl;
}

void export_triscore(){
    cout << "--- TriScore ---" << '\n';
    result_file << "--- TriScore ---" << '\n';
    triscore.execfile(instance_file);
    result_file << "Best cost : " << triscore.bestCost << '\n';
    export_order(triscore.bestOrder);
    result_file << "Time : " << triscore.time.count() << endl;
}

void export_mts(){
    cout << "--- TriScoreMargin ---" << '\n';
    result_file << "--- TriScoreMargin ---" << '\n';
    mts.delta = 3;
    mts.execfile(instance_file);
    result_file << "Delta : " << mts.delta << '\n'; 
    result_file << "Best cost : " << mts.bestCost << '\n';
    export_order(mts.bestOrder);
    result_file << "Time : " << mts.time.count() << endl;
}

void export_nts(){
    cout << "--- TriScoreNaive ---" << '\n';
    result_file << "--- TriScoreNaive ---" << '\n';
    nts.execfile(instance_file);
    result_file << "Best cost : " << nts.bestCost << '\n';
    export_order(nts.bestOrder);
    result_file << "Time : " << nts.time.count() << endl;
}

void execfile(){
    result_file << instance_file << '\n';
    display(instance_file);
    export_display();

    export_matrix();

    export_split();
    
    export_simpleg();

    export_vsplit();

    export_sideiter();

    export_sidelim();

    export_triscore();   

    export_mts();

    export_nts();
    result_file << "----------" << "\n\n";
}

void execdir(){
    string directory = "../instances/" + instance_dir + "/";
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(directory.c_str());
    if(dp == NULL){
        cerr << "Could not open directory : " << directory << '\n';
        exit(-1);
    }
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            instance_file = directory + file->d_name;
            execfile();
        }
        file = readdir(dp);
    }
    closedir(dp);
}

int main(int argc, char* argv[]){
    switch (argc){
    case 1:
        cout << "Missing argument : instance file" << '\n';
        cout << "Use 'h' to get infos" << '\n';
        return -1;
        break;
    case 2:
        if(argv[1] == string("h")){
            cout << "d directory : executes all solvers on every instance of the directory" << '\n';
            cout << "v file : displays the graph represented by this instance" << '\n';
            cout << "vd directory : displays all instances in the directory" << '\n';
            cout << "filename : execute all the algorithms on this instance" << '\n';
            cout << "Best costs marked with '*' are the optimal costs calculated using greedy algorithms" << '\n';
            cout << "The root directory's name is 'instances'" << '\n';
        }else{
            instance_file = argv[1];
            execfile();
        }
        break;
    case 3:
        if(argv[1] == string("d")){
            instance_dir = argv[2];
            execdir();
        }else if(argv[1] == string("vd")){
            display_dir(argv[2]);
        }else if(argv[1] == string("v")){
            display(argv[2]);
        }

        break;
    default:
        cout << "Error : check arguments list" << endl;
        break;
    }
    return 0;
}