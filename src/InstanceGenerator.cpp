#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <string.h>

using namespace std;

void initInstances(int n, int min, int max, int amount, char* file){
    char s [200];
    char* path = "../instances/";
    strcpy(s, path);
    strcat(s, file);
    strcat(s, "/");
    for(int i = 1; i <= amount; i++){
        std::string name = "instance_" + to_string(i) + "_" + to_string(n) + ".col";
        std::ofstream file(s + name);

        if(file.is_open()){
            //c name
            //c Source
            file << "c FILE: " << name <<endl;
            file << "c SOURCE: initInstances" << endl;
            file << "c min : " << min << endl;
            file << "c max : " << max << endl;

            //p carac
            file << "p " << n << endl;
            
            //e edges
            //tant qu'on a pas placé ..., on fait
            if(n > 3){
                file << "e 0 1 " << min + rand()%(max - min) << endl;
                file << "e 0 " << n/2 << " " << min + rand()%(max - min) << endl;
                for(int i = 1; i < n/2 - 1; i++){
                    file << "e " << i << " " << i+1 << " " << min + rand()%(max - min) << endl;
                    file << "e " << i << " " << i+n/2 << " " << min + rand()%(max - min) << endl;
                }
                file << "e " << n/2 - 1 << " " << n - 1 << " " << min + rand()%(max - min) << endl;
                for(int i = n/2; i < n - 1; i++){
                    file << "e " << i << " " << i+1 << " " << min + rand()%(max - min) << endl;
                }
            }
        }else{
            printf("ERROR: Cannot open outfile\n");
        }
    }
}


int main(int argc, char* argv[]){
    switch (argc)
    {
    case 2:
        initInstances(stoi(argv[1]), 1, 10, 1, "autre");
        break;
    case 3:
        initInstances(stoi(argv[1]), max(1, stoi(argv[2])), stoi(argv[2])+10, 1, "autre");
        break;

    case 4:
        initInstances(stoi(argv[1]), max(1, stoi(argv[2])), stoi(argv[3]), 1, "autre");
        break;

    case 5:
        initInstances(stoi(argv[1]), max(1, stoi(argv[2])), stoi(argv[3]), stoi(argv[4]), "autre");
        break;
    
    case 6:
        initInstances(stoi(argv[1]), max(1, stoi(argv[2])), stoi(argv[3]), stoi(argv[4]), argv[5]);
    default:
        initInstances(6, 1, 10, 1, "small");
        break;
    }
}