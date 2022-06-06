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

    char* mot;
    int n_e = 0;
    int n_p = 0;
    int nL = 0;
    int padding[100] = {0}; //should be enough (I need size/2-1 slots)
    //you start with a spacing of 4, and add 1 for each extra character

    string output[3];
    //first step is filling the strings and getting the right padding

    //preamble
    while(getline(input_file, line) && line[0] != 'v'){
        if(line.size() > 2){
            cout << &line[2] << '\n';
        }
    }

    //view
    do{
        n_p = 0;
        istringstream flux(&line[2]);
        while(flux >> mot){
            int size;
            if(mot[0] == '*'){
                output[nL].append(red + &mot[1] + blue + "(" + to_string(n_e) + ")");
                size = strlen(&mot[1]) + to_string(n_e).size() + 1;
                n_e++;
            }else{
                output[nL].append(mot);
                size = strlen(mot) - 1;
            }
            padding[n_p] = max(padding[n_p], size);
            output[nL].append(" ");
        }
        n_p++;
        nL++;
    }while(getline(input_file, line) && line[0] == 'v');

    for(string s : output){

    }
    //parameters and infos
    do{
        switch(line[0]){
            case 'p':
                cout << "Size : " << &line[2] << '\n';
                break;
            case 'r':
                if(line.size() > 2){
                    cout << "Best cost : " << &line[2] << '\n';
                }
                break;
            case 'o':
                if(line.size() > 2){
                    cout << "Best order : " << &line[2] << '\n';
                }
        }
    }while(getline(input_file, line) && line[0] != 'e');
    
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