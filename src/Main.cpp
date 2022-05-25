#include "Matrice/MatrixSolver.hpp"
#include "RangeSplit/RangeSplitSolver.hpp"
#include "Side/SideExSolver.hpp"
#include "Split/SplitSolver.hpp"
#include "TriScore/TriScore.hpp"

MatrixSolver matrix;
SplitRange splitrange;
MonoSide side;
Split split;
TriScore triscore;

void display(const char* file){
    cout << "---------------" << endl;
    char path[100] = "../instances/";
    strcat(path, file);
    string filename(path);
    cout << file << endl;
    string line;

    string red("\033[0;31m");
    string reset("\033[0m");
    
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        exit(-1);
    }else{
        while (getline(input_file, line) && line[0] != 'e'){
            for(auto& c : line){
                switch(c){
                    case '*':
                        cout << red;
                        break;
                    case ' ':
                        cout << reset << c;
                        break;
                    case '\n':
                        cout << reset << c;
                        break;
                    case '\0':
                        cout << reset << c;
                        break;
                    case 'v':
                        cout << reset;
                        break;
                    default:
                        cout << c;
                        break;
                }
            }
            cout << endl;
        }
    }

    input_file.close();
    cout << "---------------" << endl;
}

void displaydir(const char* dir){
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

            display(dir);
        }
        file = readdir(dp);
    }
    closedir(dp);
}

void execfile(const char* file){
    display(file);
    
    //cout << "--- Matrix ---" << endl;
    //matrix.execfile(file);

    //cout << "--- Split ---" << endl; 
    //split.execfile(file);

    cout << "--- SplitRange ---" << endl;
    splitrange.delta = 3;
    splitrange.execfile(file);

    cout << "--- Side ---" << endl;
    side.execfile(file);

    cout << "--- TriScore ---" << endl;
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
        //}else if(argv[1] == c_str('vd')){
        //    displaydir(argv[2]);
        }else if(*argv[1] == 'v'){
            display(argv[2]);
        }
        break;
    default:
        cout << "Error : check arguments list" << endl;
        break;
    }
    return 0;
}