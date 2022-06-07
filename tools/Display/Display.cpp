#include "Display.hpp"
using namespace std;

void display2(const char* file){
    cout << "---------------" << '\n';
    char path[100] = "../instances/";
    strcat(path, file);
    string filename(path);
    //cout << file << '\n';
    string line;

    string red("\033[0;31m");
    string reset("\033[0m");
    
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        exit(-1);
    }else{
        int n = 0;
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

void display(const char* file){
    cout << "---------------" << '\n';
    char path[100] = "../instances/";
    strcat(path, file);
    string filename(path);
    //cout << file << '\n';
    string line;

    string red("\033[0;31m");
    string blue("\033[0;34m");
    string reset("\033[0m");
    
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
                            cout << red << &mot[1] << blue << "(" << n << ")" << padding;
                            n++;
                        }else{
                            cout << reset << mot << "   ";
                        }
                    }
                    nL++;
                break;
                case 'p':
                    cout << "Sommets : " << &line[2];
                break;
                default:
                    cout << &line[2];
                break;
            }
            cout << '\n';
        }
    }
}

void display_dir(const char* dir){
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

            display(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}