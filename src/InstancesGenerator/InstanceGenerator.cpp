#include "InstanceGenerator.hpp"

void initInstances(int n, int min, int max, int amount, char* file){
    char s [200];
    char* path = "../instances/";
    strcpy(s, path);
    strcat(s, file);
    strcat(s, "/");
    for(int i = 1; i <= amount; i++){
        string name = "instance_" + to_string(i) + "_" + to_string(n) + ".col";
        ofstream file(s + name);
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
                for(int i = 0; i < n/2 - 1; i++){
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

    for(int i = 1; i < argc - 1; i++){
        switch(*argv[i]){
            case 'm':
                m = atoi(argv[i+1]);
                i++;
                break;
            case 'M':
                M = atoi(argv[i+1]);
                i++;
                break;
            case 'n':
                amount = atoi(argv[i+1]);
                i++;
                break;
            case 's':
                size = atoi(argv[i+1]);
                i++;
                break;
            case 'd':
                dir = argv[i+1];
                i++;
                break;
        }
    }
    initInstances(size, m, M, amount, dir);
    cout << "Generation complete" << endl;
}